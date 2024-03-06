/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:32 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:54:33 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
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

#include "../server/Colors.hpp"
#include "../config/Location.hpp"

#define SIZE 1024

class Request {

    private:
        int sendedcontent;
        std::string recString;
        std::string method;
        std::string path;
        std::string connection;
        std::string header;
        std::string body;

    public:
        Request( void );
        ~Request( void );
        int getsendedcontent( void ) const;
        void setsendedcontent( int const &sendedcontent );
        void setRecString( std::string recString );
        std::string getRecString( void );
        void setMethod( std::string method );
        std::string getMethod( void );
        void setPath( std::string Path );
        std::string getPath( void );
        std::string getHeader( void );
        void setHeader( std::string header );
        std::string getBody( void );
        void setBody( std::string body );
        std::string getConnection( void );
        void setConnection( std::string connection );

        int getRequestBodySize( void );
        void parseRequest( void );
        int setRequestHeader( void );
        void parseRequestHeader( void );
        std::string getValue( std::string key );
};