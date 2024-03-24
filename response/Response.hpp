/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:22 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/24 03:17:12 by del-yaag         ###   ########.fr       */
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

#include <dirent.h>
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

#define SEND 1024
#define CHUNKED 2621440

class Request;

class Response {

    private:
        int receivedcontent;
        int statusCode;
        int file;
        bool sendedHeader;
        bool displayError;
        bool autoIndexing;
        size_t contentLength;
        size_t countBytesRead;
        size_t contentResponse;
        std::string type;
        std::string mimeType;

        // post method
        std::string body;
        std::string BHName;
        std::string BHFilename;
        std::string BHContentDispo;
        std::string BHContentType;
        int         fd;
        bool        bodyFlag;

    public:
        Response( void );
        ~Response( void );
        int getreceivedcontent( void ) const;
        void setreceivedcontent( int const & receivedcontent );
        void setStatusCode( int const & statusCode );
        int getStatusCode( void ) const;
        void setSendedHeader( bool const & sendedHeader );
        bool getSendedHeader( void ) const;
        void setDisplayError( bool const & displayError );
        bool getDisplayError( void ) const;
        void setAutoIndexing( bool const & autoIndexing );
        bool getAutoIndexing( void ) const;
        void setContentLength( size_t const & contentLength );
        size_t getContentLength( void ) const;
        void setCountBytesRead( size_t const & countBytesRead );
        size_t getCountBytesRead( void ) const;
        void setContentResponse( size_t const & contentResponse );
        size_t getContentResponse( void ) const;
        void setFile( int const & file );
        int getFile( void ) const;
        std::string getType( void ) const;
        void setType( std::string const & type );
        std::string getMimeType( void ) const;
        void setMimeType( std::string const & mimetype );

        ssize_t sendHeader( int const &sockfd, Request const & request);
        ssize_t sendBody( int const &sockfd, Request const & request);
        std::string getStatusMessage(int const & statusCode);
        void displayErrorPage( Conf::Server & server, int const &sockfd, Request request );
        int displayAutoIndex( Conf::Server & server, int const &sockfd, Request request );
        std::string getErrorPage(std::map<std::string, std::string> ErrorPages);

        // ------------ post ------------ //
        void parsePostBodyHeader( std::string const &chunck );
        int parseEncodingBody( void );
        int parseBoundariesBody( Request const &request );
        int parseLengthBody( void );
        std::string getHeaderValue( std::string const &chunck, std::string const &findStr, std::string const &delim );
        int createFileAndWrite( std::string const &str, bool const flag );
        int execPostMethod( Request const &request );
        int PutChunkedBodyToFile( Request const &request, bool const flag );
        void resetHeaderElements( void );
        
        std::string getBHName( void ) const;
        void setBHName( std::string const &name );
        std::string getBHFilename( void ) const;
        void setBHFilename( std::string const &filename );
        std::string getBHContentDispo( void ) const;
        void setBHContentDispo( std::string const &content );
        std::string getBHContentType( void ) const;
        void setBHContentType( std::string const &type );
        std::string getBody( void ) const;
        void setBody( std::string const &body );
};

std::string getMimeType(const std::string& extension);
int hexadecimalToDecimal( std::string hexVal );