/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:32 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/18 18:11:20 by del-yaag         ###   ########.fr       */
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
#include <vector>
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
#include "../config/Config.hpp"

#define SIZE 1024

class Response;

class Request {

    private:
        int sendedcontent;
        std::string recString;
        std::string method;
        std::string path;
        std::string connection;
        std::string header;
        std::string body;
        std::string rangeStart;
        std::string rangeEnd;
        std::string httpVersion;
        size_t rangeStartNum;
        size_t rangeEndNum;
        std::map<std::string, std::string> linesRequest;
        std::string url;

        // location
        Location location;
        bool checkLocation;
        std::string stringLocation;

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
        std::string getRangeStart( void ) const;
        void setRangeStart( std::string const & rangeStart );
        std::string getRangeEnd( void ) const;
        void setRangeEnd( std::string const & rangeEnd );
        size_t getRangeStartNum( void ) const;
        void setRangeStartNum( size_t const & rangeStartNum );
        size_t getRangeEndNum( void ) const;
        void setRangeEndNum( size_t const & rangeEndNum );
        void setHttpVersion ( std::string const & httpVersion );
        std::string getHttpVersion( void ) const;
        std::map<std::string, std::string> const & getLinesRequest( void ) const;
        Location const & getLocation( void ) const;
        void setLocation( Location const & location );
        bool getCheckLocation( void ) const;
        void setCheckLocation( bool checkLocation ) ;
        std::string getStringLocation( void ) const;
        void setStringLocation( std::string const & stringLocation );
        std::string getUrl( void ) const;
        void setUrl( std::string url );


        size_t getRequestBodySize( void ) const;
        int setRequestHeader( void );
        int parseRequestHeader(Config conf, Conf::Server & server, Response & response );
        void setRequestBody( void );
        std::string getValue( std::string const & key ) const;
        std::string getIndex( std::vector<std::string> const & indexes, std::string const & root ) const ;
        int parseRequestLine( Config conf, Conf::Server & server, Response & response );
        int checkMethod( Response & response );
        int setMapRequestLines( Response & response );

        // Get Method
        void getRange( void );
        int checkFile( Conf::Server & server, Response & response );
        int checkDirectory( Conf::Server & server, Response & response );
};
