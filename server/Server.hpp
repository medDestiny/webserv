/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:26:06 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/04 20:51:46 by mmisskin         ###   ########.fr       */
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
#include "../config/Config.hpp"

#define SEND 1024
#define BACKLOG 128

class Server {

    private:
        std::set<int> serverfds;
        std::set<std::pair<std::string, std::string> > donehp;
        int yes;
        int status;
        
        struct addrinfo hints;
        struct addrinfo *addrInfo;
        struct addrinfo *newinfo;

        // accept
        // struct sockaddr_storage ramoteaddr;
        // socklen_t addrlen;
        // char remoteip[INET6_ADDRSTRLEN];
        
        // // poll
        std::vector<struct pollfd> pfds;

        std::vector<Conf::Server> servers;


        // members methods
        void getInfoaddr( std::string const &host, std::string const &port );
        int createsocket( int &listener );
        void bindlistensock( int &listener, std::vector<Conf::Server>::iterator &it );
        void addpollservers( void );
        void addpollclients( int const &fd );
        void mainpoll( void );
    
    public:
        Server( Config const &config );
        ~Server( void );

        void createServer( void );

};

void printvalidoption( std::string const &str );
void printinvalidopt( std::string const &str );
