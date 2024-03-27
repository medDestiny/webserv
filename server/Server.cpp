/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoukhle <amoukhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:55:40 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/25 00:48:26 by amoukhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( Config const &config ) {

    this->yes = 1;
    this->addrInfo = NULL;
    this->newinfo = NULL;
    this->config = config;
    this->servers = this->config.getServers();
}

Server::~Server( void ) { }

void Server::getInfoaddr( std::string const &host, std::string const &port ) {

    int status;

    std::memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    status = getaddrinfo( host.c_str(), port.c_str(), &hints, &addrInfo );
    if ( status == -1 ) {

        std::cout << "error: getaddrinfo: " << gai_strerror( status ) << std::endl;
        exit( EXIT_FAILURE );
    }
    printvalidoption( "getaddrinfo" );
}

int Server::createsocket( int &listener ) {

    listener = socket( newinfo->ai_family, newinfo->ai_socktype, newinfo->ai_protocol );
    if ( listener == -1 ) {

        std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
        return -1;
    }
    printvalidoption( "socket" );

    if ( fcntl( listener, F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 ) {

        std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
        return -1;
    }
    printvalidoption( "fcntl" );

    if ( setsockopt( listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == -1 ) {

        std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
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
            std::cout << RED << "\t==> ERROR: " << strerror( errno ) << RESET << std::endl;
            continue;
        }
        printvalidoption( "bind" );
        break;
    }
    freeaddrinfo( addrInfo );

    if ( !newinfo ) {

        // print the error of not bound address
        std::string error = strerror( errno );
        printinvalidopt(    "==> ERROR: " "socket '" + std::to_string( listener ) +
                            "' didn't get bound with '" + it->getListen().getHost() +
                            ":" + it->getListen().getPort() + "' because: " + error );
        exit( EXIT_FAILURE );
    }

    if ( listen( listener, BACKLOG ) == -1 ) {

        std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
        exit( EXIT_FAILURE );
    }
    printvalidoption( "listen" );
    serverfds[listener] = *it;
}

int Server::alreadyboundsock( std::vector<Conf::Server>::iterator const &server ) {

    if ( this->donehp.size() > 0 ) {
        
        std::pair<std::string, std::string> search = std::make_pair( server->getListen().getHost(), server->getListen().getPort() );
        std::set<std::pair<std::string, std::string> >::iterator it = donehp.find( search );
        if ( it != donehp.end() ) {

            printinvalidopt( "==> Seems like this address is already bound " + it->first + ":" + it->second );
            std::cout << std::endl;
            return 1;
        }
    }
    return 0;
}

void Server::createServer( void ) {

    int listener;  

    std::vector<Conf::Server>::iterator it = servers.begin();
    for ( ; it != servers.end(); ++it ) {

        if ( this->alreadyboundsock( it ) )
            continue;

        // create socket and bind it
        this->getInfoaddr( it->getListen().getHost(), it->getListen().getPort() );
        this->bindlistensock( listener, it );

        // add bound host, port to the container
        donehp.insert( std::make_pair( it->getListen().getHost(), it->getListen().getPort() ) );
        std::cout << DYELLO << "\tserver: " << it->getListen().getHost() << ":" << it->getListen().getPort() << RESET << std::endl << std::endl;
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
            if ( clientit != clients.end() ) {
                clientit->second.setserver( server );
                clientit->second.setConfig( config );
            }
        }
    }
}

int Server::acceptconnections( int const &sockfd, Conf::Server server ) {

    int newfd;

    this->addrlen = sizeof( this->remoteaddr );
    newfd = accept( sockfd, ( struct sockaddr * )&remoteaddr, &addrlen );
    if ( newfd == -1 ) {

        if ( errno == ECONNABORTED || errno == EAGAIN ) {

            std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
            return -1;
        }
        std::cout << RED << "accept fail" << RESET << std::endl;
        return (-1);
    } else {

        if ( fcntl( sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 ) {

            std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
            std::cout << RED << "fcntl fail" << RESET << std::endl;
            return (-1);
        }
        this->addpollclients( newfd );
        this->addclients( newfd, server );
        this->printConeectedaddr( server, newfd );
    }
    return 0;
}

void Server::pollwithtimeout( void ) {

    int status;

    status = poll( pfds.data(), pfds.size(), TIMEOUT );
    if ( status == -1 ) {

        std::cout << RED << "==> ERROR: " << strerror( errno ) << RESET << std::endl;
        exit( EXIT_FAILURE );
    } else if ( !status )
        this->checkclienttimeout();
}

void Server::mainpoll( void ) {

    std::map<int, Conf::Server>::iterator it;
    std::map<int, Client>::iterator itClient;
    
    this->pollwithtimeout();
    for ( size_t i = 0; i < pfds.size(); i++ ) {
        it = serverfds.find( pfds[i].fd );
        if ( pfds[i].revents == POLLIN ) {
            if ( it != serverfds.end() ) {
                if ( this->acceptconnections( pfds[i].fd, it->second ) == -1 )
                    continue;
            } else {

                // POLLIN revent int the clients side
                itClient = clients.find( pfds[i].fd );
                itClient->second.settimeout( std::time(NULL) );
                if ( itClient->second.recieveRequest( pfds[i].fd ) == 0 ) {
                    pfds[i].events = POLLOUT;
                }
            }
        } else if ( pfds[i].revents == POLLOUT ) {

            if ( it != serverfds.end() ) {

                // POLLOUT revent in the server side
            } else {
                // POLLOUT revents in the clients side
                int var;
                itClient = clients.find( pfds[i].fd );
                itClient->second.settimeout( std::time(NULL) );
                var = itClient->second.sendresponse( pfds[i].fd );
                if ( !var ) {
                    this->removeclient( pfds[i].fd );
                    this->removepollclient( i );
                }
                else if (var == 2) {
                    pfds[i].events = POLLIN;
                    Client client(itClient->second);
                    this->removeclient( pfds[i].fd );
                    clients[pfds[i].fd] = client;
                }
            }
        } else if ( pfds[i].revents == POLLHUP ) {
            this->removeclient( pfds[i].fd );
            this->removepollclient( i );
        }
        else
            this->checkclienttimeout();
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
                // std::cout << clients.size() << " " << pfds.size();
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

void Server::printConeectedaddr ( Conf::Server const &server, int const &sockfd ) {

    std::cout
        << DBLUE 
        << "\t--> connection accepted: "
        << server.getListen().getHost()
        << ":"
        << server.getListen().getPort()
        << " on "
        << sockfd
        << RESET
        << std::endl;
}
