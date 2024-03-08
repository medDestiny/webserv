/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:22 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:54:24 by del-yaag         ###   ########.fr       */
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

#include "../server/Colors.hpp"
#include "../config/Location.hpp"

class Request;

class Response {

    private:
        int receivedcontent;
        int statusCode;
        int file;
        bool sendedHeader;
        bool notFound;
        size_t contentLength;

    public:
        Response( void );
        ~Response( void );
        int getreceivedcontent( void ) const;
        void setreceivedcontent( int const & receivedcontent );
        void setStatusCode( int const & statusCode );
        int getStatusCode( void ) const;
        void setSendedHeader( bool const & sendedHeader );
        bool getSendedHeader( void ) const;
        void setNotFound( bool const & notFound );
        bool getNotFound( void ) const;
        void setContentLength( size_t const & contentLength );
        size_t getContentLength( void ) const;
        void setFile( int const & file );
        int getFile( void ) const;

        void sendHeader( Conf::Server const & server, int const &sockfd, Request const & request);
        void sendBody( Conf::Server const & server, int const &sockfd, Request const & request);
        std::string getStatusMessage(int const & statusCode);

};

std::string getMimeType(const std::string& extension);