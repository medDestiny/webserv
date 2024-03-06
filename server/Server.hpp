/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:26:06 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:46:23 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#include <set>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/fcntl.h>
#include <errno.h>

#include "Colors.hpp"
#include "../config/Server.hpp"
#include "../config/Location.hpp"
#include "../config/Config.hpp"
#include "../client/Client.hpp"

#define SEND 1024
#define BACKLOG 128
#define TIMEOUT 2000

class Client;

class Server {

    private:
        std::set<std::pair<std::string, std::string> > donehp;
        Config config;
        int yes;
        
        struct addrinfo hints;
        struct addrinfo *addrInfo;
        struct addrinfo *newinfo;

        // accept
        struct sockaddr_storage remoteaddr;
        socklen_t addrlen;
        // char remoteip[INET6_ADDRSTRLEN];
        
        // poll servers clients
        std::vector<struct pollfd> pfds;
        std::vector<Conf::Server> servers;
        std::map<int, Client> clients;
        std::map<int, Conf::Server> serverfds;



        // socket methods
        void getInfoaddr( std::string const &host, std::string const &port );
        int createsocket( int &listener );
        void bindlistensock( int &listener, std::vector<Conf::Server>::iterator &it );
        int alreadyboundsock( std::vector<Conf::Server>::iterator const &server );

        // poll methods
        void addpollservers( void );
        void addpollclients( int const &fd );
        void removepollclient( int const &index );
        void searchandremovepollclient( int const &sockfd );
        void mainpoll( void );
        void pollwithtimeout( void );
        
        // client methods
        void addclients( int const &sockfd, Conf::Server const &server );
        void removeclient( int const &sockfd );
        void checkclienttimeout( void );
        int acceptconnections( int const &sockfd, Conf::Server server );

        //----------- debug -----------//
        void printConeectedaddr ( Conf::Server const &server, int const &sockfd );
    
    public:
        Server( Config const &config );
        ~Server( void );

        void createServer( void );

};

//----------- debug -----------//
void printvalidoption( std::string const &str );
void printinvalidopt( std::string const &str );
