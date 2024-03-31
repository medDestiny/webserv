/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoukhle <amoukhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:32 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/31 06:44:21 by amoukhle         ###   ########.fr       */
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
#include "../cgi/Cgi.hpp"

#define SIZE 1024

enum bodyType {

    LENGTH,
    ENCODING,
    BOUNDARIES
};

class Response;

class Request {

    private:
        int 		sendedcontent;
        std::string	recString;
        std::string method;
        std::string path;
        std::string connection;
        std::string header;
        std::string body;
        std::string rangeStart;
        std::string rangeEnd;
        std::string httpVersion;
        std::string url;
        size_t		rangeStartNum;
        size_t 		rangeEndNum;
        std::map<std::string, std::string> linesRequest;

        // post method
        std::string startBoundary;
        std::string endBoundary;
        std::string contentType;
        std::string postBodyHeader;
        std::string transferEncoding;
        int         bodyType;
        size_t      contentLength;
        
        // location
        Location	location;
        bool		checkLocation;
        std::string	stringLocation;

        // return
        int returnCode;
        std::string returnUrl;
		// Cgi
		Cgi			cgi;

    public:
        Request( void );
        ~Request( void );
        int			getsendedcontent( void ) const;
        void		setsendedcontent( int const &sendedcontent );
        void 		setRecString( std::string const & recString );
        std::string	getRecString( void ) const ;
        void		setMethod( std::string const & method );
        std::string	getMethod( void ) const;
        void		setPath( std::string const & Path );
        std::string	getPath( void ) const;
        std::string	getHeader( void ) const;
        void		setHeader( std::string const & header );
        std::string	getBody( void ) const;
        void		setBody( char const *body, int const &size );
        void        setBodyForCgi( std::string const &body );
        std::string	getConnection( void ) const;
        void		setConnection( std::string const & connection );
        std::string	getRangeStart( void ) const;
        void		setRangeStart( std::string const & rangeStart );
        std::string	getRangeEnd( void ) const;
        void		setRangeEnd( std::string const & rangeEnd );
        size_t		getRangeStartNum( void ) const;
        void		setRangeStartNum( size_t const & rangeStartNum );
        size_t		getRangeEndNum( void ) const;
        void		setRangeEndNum( size_t const & rangeEndNum );
        void		setHttpVersion ( std::string const & httpVersion );
        std::string	getHttpVersion( void ) const;
        std::map<std::string, std::string> const & getLinesRequest( void ) const;
        Location const & getLocation( void ) const;
        void		setLocation( Location const & location );
        bool 		getCheckLocation( void ) const;
        void 		setCheckLocation( bool checkLocation ) ;
        std::string	getStringLocation( void ) const;
        void		setStringLocation( std::string const & stringLocation );
        std::string	getUrl( void ) const;
        void		setUrl( std::string url );
        int         getReturnCode( void ) const;
        void        setReturnCode( int returnCode );
        std::string getReturnUrl( void ) const;
        void        setReturnUrl( std::string const & returnUrl );

		/* cgi functions */
		bool		handleCgiRequest(std::string const & root, std::string const & location, std::string const & cgi, Response & response);
		void		setCgiFiles(std::string const & suffix);
		bool		isCgi(void) const;
		Cgi &		getCgi(void);

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

        // ------------- POST ------------- //
        int parsePostBody( std::string const &buffer );
        int bufferPostBody( std::string const &buffer );
        void parsePostHeader( void );
        
        std::string getStartBoundary( void ) const;
        void setStartBoundary( std::string const &boundary );
        std::string getEndBoundary( void ) const;
        void setEndBoundary( std::string const &boundary );
        std::string getContentType( void ) const;
        void setContentType( std::string const &type );
        size_t getContentLength( void ) const;
        void setContentLength( size_t const &length );
        std::string getPostBodyHeader( void ) const;
        void setPostBodyHeader( std::string const &header );
        std::string getTransferEncoding( void ) const;
        void setTransferEncoding( std::string const &content );
        int getBodyType( void ) const;
        void setBodyType( int const &type );
};
