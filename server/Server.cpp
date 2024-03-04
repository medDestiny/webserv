#include "Server.hpp"

Server::Server( Config const &config ) {

    this->yes = 1;
    this->addrInfo = NULL;
    this->newinfo = NULL;
    this->servers = config.getServers();
}

Server::~Server( void ) { }

void Server::getInfoaddr( std::string const &host, std::string const &port ) {

    std::memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    this->status = getaddrinfo( host.c_str(), port.c_str(), &hints, &addrInfo );
    if ( this->status == -1 ) {

        std::cout << "error: getaddrinfo: " << gai_strerror( status ) << std::endl;
        exit( EXIT_FAILURE );
    }
    printvalidoption( "getaddrinfo" );

}

int Server::createsocket( int &listener ) {

    listener = socket( newinfo->ai_family, newinfo->ai_socktype, newinfo->ai_protocol );
    if ( listener == -1 ) {

        perror( "socket" );
        return -1;
    }
    printvalidoption( "socket" );

    // std::cout << listener << std::endl;
    if ( fcntl( listener, F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 ) {

        perror( "fcntl" );
        return -1;
    }
    printvalidoption( "fcntl" );

    if ( setsockopt( listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == -1 ) {

        perror( "setsockopt" );
        return -1;
    }
    printvalidoption( "setsockopt" );

    return 0;
}

void Server::bindlistensock( int &listener, std::vector<Conf::Server>::iterator &it ) {

    ( void ) it;
    for ( newinfo = addrInfo; newinfo; newinfo = newinfo->ai_next ) {

        if ( this->createsocket( listener ) == -1 )
            continue;
        
        if ( bind( listener, newinfo->ai_addr, newinfo->ai_addrlen ) < 0 ) {

            close( listener );
            perror( "bind" );
            continue;
        }
        printvalidoption( "bind" );
        break;
    }
    freeaddrinfo( addrInfo );

    if ( !newinfo ) {

        printinvalidopt( "didn't get bound" );
        exit( EXIT_FAILURE );
    }

    if ( listen( listener, BACKLOG ) == -1 ) {

        perror( "listen" );
        exit( EXIT_FAILURE );
    }
    printvalidoption( "listen" );
    serverfds[listener] = *it;
}

void Server::createServer( void ) {

    int listener;

    std::vector<Conf::Server>::iterator it = servers.begin();
    for ( ; it != servers.end(); ++it ) {

        // check if the host, port is already bound
        if ( donehp.size() > 0 ) {

            std::pair<std::string, std::string> search = std::make_pair( it->getListen().getHost(), it->getListen().getPort() );
            std::set<std::pair<std::string, std::string> >::iterator it = donehp.find( search );
            if ( it != donehp.end() ) {

                printinvalidopt( "** seems like this socket already bound " + it->first + ":" + it->second );
                std::cout << std::endl;
                continue;
            }
        }

        // create socket and bind it
        this->getInfoaddr( it->getListen().getHost(), it->getListen().getPort() );
        this->bindlistensock( listener, it );

        // add bound host, port to the container
        donehp.insert( std::make_pair( it->getListen().getHost(), it->getListen().getPort() ) );
        std::cout << DYELLO << "\tserver: " << it->getListen().getHost() << ":" << it->getListen().getPort() << RESET << std::endl;
        std::cout << std::endl;
    }
    this->addpollservers();

    while ( 1 ) {
        
        this->mainpoll();
    }
}

void Server::addpollservers( void ) {

    struct pollfd pfd;
    std::map<int, Conf::Server>::iterator it = serverfds.begin();
    for ( ; it != serverfds.end(); ++it ) {

        std::cout << "sockfd: " << it->first << " " << it->second.getListen().getHost() << ":" << it->second.getListen().getPort() << std::endl;
        pfd.fd = it->first; 
        pfd.events = POLLIN;
        pfds.push_back( pfd );
    }
}

void Server::addpollclients( int const &fd ) {

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    pfds.push_back( pfd );
}

void Server::addclients( int const &sockfd, Conf::Server const &server ) {

    Client client;
    std::map<int, Client>::iterator it;

    client.setsockfd( sockfd );
    client.settimeout( std::time(NULL) );

    if ( clients.size() == 0 ) {

        std::cout << DMAGENTA << "\t-> add first client" << RESET << std::endl;
        clients[sockfd] = client;
        std::map<int, Client>::iterator clientit = clients.find( sockfd );
        if ( clientit != clients.end() )
            clientit->second.setserver( server );
    } else {

        it = clients.find( sockfd );
        if ( it == clients.end() ) {

            std::cout << MAGENTA << "\t-> add more clients" << RESET << std::endl;
            clients[sockfd] = client;
            std::map<int, Client>::iterator clientit = clients.find( sockfd );
            if ( clientit != clients.end() )
                clientit->second.setserver( server );
        }
    }
}

int Server::acceptconnections( int const &sockfd, Conf::Server server ) {

    int newfd;

    this->addrlen = sizeof( this->remoteaddr );
    newfd = accept( sockfd, ( struct sockaddr * )&remoteaddr, &addrlen );
    if ( newfd == -1 ) {

        if ( errno == ECONNABORTED || errno == EAGAIN ) {

            perror( "accept" );
            return -1;
        }
        exit( EXIT_FAILURE );
    } else {

        if ( fcntl( sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 ) {

            perror( "fcntl" );
            exit( EXIT_FAILURE );
        }
        this->addpollclients( newfd );
        this->addclients( newfd, server );
        this->printConeectedaddr( newfd );
    }
    // printvalidoption( "accept" );
    return 0;
}

void Server::pollwithtimeout( void ) {

    int status;

    status = poll( pfds.data(), pfds.size(), TIMEOUT );
    if ( status == -1 ) {

        perror( "poll" );
        exit( EXIT_FAILURE );
    } else if ( !status )
        this->checkclienttimeout();
}

void Server::mainpoll( void ) {

    std::map<int, Conf::Server>::iterator it;
    
    this->pollwithtimeout();
    for ( size_t i = 0; i < pfds.size(); i++ ) {

        it = serverfds.find( pfds[i].fd );
        if ( pfds[i].revents == POLLIN ) {

            if ( it != serverfds.end() ) {
                if ( this->acceptconnections( pfds[i].fd, it->second ) == -1 )
                    continue;
            } else {

                // std::cout << "hello from pollin" << std::endl;
                // POLLIN revent int the clients side
            }
        } else if ( pfds[i].revents == POLLOUT ) {

            if ( it != serverfds.end() ) {

                // std::cout << "hello from pollout server side" << std::endl;
                // POLLOUT revent in the server side
            } else {

                // std::cout << pfds.size() << " hello from pollout" << std::endl;
                // POLLOUT revents in the clients side
            }
        } else if ( pfds[i].revents == POLLHUP ) {

            // std::cout << "hello from pullhup" << std::endl;
        } else {

            // std::cout << "hello from else" << std::endl;
            this->checkclienttimeout();
        }
    }

}

void Server::checkclienttimeout( void ) {

    if ( !clients.empty() ) {

        std::map<int, Client>::iterator it = clients.begin();
        std::time_t now = std::time(NULL);
        std::time_t diff;
        std::time_t start;
        for ( ; it != clients.end(); ++it ) {

            start = it->second.gettimeout();
            diff = now - start;
            if ( diff >= 10 ) {

                this->searchandremovepollclient( it->second.getsockfd() );
                clients.erase( it );
                std::cout << clients.size() << " " << pfds.size();
                printinvalidopt( "-> client has been deleted " );
                if ( clients.empty() )
                    break;
                it = clients.begin();
            }
        }
    }
}

void Server::searchandremovepollclient( int const &sockfd ) {

    std::vector<struct pollfd>::iterator it = pfds.begin();
    for ( ; it != pfds.end(); ++it )
        if ( it->fd == sockfd )
            break;
    if ( it != pfds.end() ) {

        close( it->fd );
        pfds.erase( it );
        printinvalidopt( "-> connection closed with the client TIMEOUT" );
    }
}

void Server::removeclient( int const &sockfd ) {

    std::map<int, Client>::iterator it = clients.find( sockfd );
    if ( it != clients.end() ) {

        clients.erase( it->first );
        std::cout << MAGENTA << "\t->client has been deleted" << RESET << std::endl;
    }
}

void Server::removepollclient( int const &index ) {

    std::vector<struct pollfd>::iterator it = pfds.begin() + index;
    close( pfds[index].fd );
    pfds.erase( it );
    std::cout << MAGENTA << "\t->poll client has been deleted" << RESET << std::endl;
}

void printvalidoption( std::string const &str ) {

    std::cout << GREEN << "\t--> " << str << RESET << std::endl;
}

void printinvalidopt( std::string const &str ) {

    std::cout << RED << "\t" << str << RESET << std::endl;
}

void *Server::getinaddr( struct sockaddr *sa ) {

    if ( sa->sa_family == AF_INET )
        return &( ( struct sockaddr_in * )sa )->sin_addr;
    return &( ( struct sockaddr_in6 * )sa )->sin6_addr;
}

void Server::printConeectedaddr ( int const &sockfd ) {

    std::cout
        << GREEN 
        << "\t--> connection accepted: "
        << inet_ntop(
                        remoteaddr.ss_family, 
                        this->getinaddr( ( struct sockaddr * )&remoteaddr ),
                        remoteip, INET6_ADDRSTRLEN
                    )
        << " on "
        << sockfd
        << RESET
        << std::endl;
}