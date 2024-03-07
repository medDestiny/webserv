/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:32 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/07 14:09:43 by mmisskin         ###   ########.fr       */
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
        void setRecString( std::string const & recString );
        std::string getRecString( void ) const ;
        void setMethod( std::string const & method );
        std::string getMethod( void ) const;
        void setPath( std::string const & Path );
        std::string getPath( void ) const;
        std::string getHeader( void ) const;
        void setHeader( std::string const & header );
        std::string getBody( void ) const;
        void setBody( std::string const & body );
        std::string getConnection( void ) const;
        void setConnection( std::string const & connection );

        int getRequestBodySize( void ) const;
        void parseRequest( void );
        int setRequestHeader( void );
        void parseRequestHeader( Conf::Server & server );
        std::string getValue( std::string const & key ) const;
};
