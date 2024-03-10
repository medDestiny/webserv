/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:29 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/07 14:03:40 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../response/Response.hpp"
#include "../client/Client.hpp"

Request::Request( void ) {

    this->sendedcontent = 0;
}

Request::~Request( void ) { }

int Request::getsendedcontent( void ) const {

    return this->sendedcontent;
}

void Request::setsendedcontent( int const &sendedcontent ) {

    this->sendedcontent = sendedcontent;
}

void Request::setRecString( std::string const & recString ) {

    this->recString += recString;
}
std::string Request::getRecString( void ) const {

    return (this->recString);
}

void Request::setMethod( std::string const & method ) {

    this->method = method;
}
std::string Request::getMethod( void ) const {

    return (this->method);
}

void Request::setPath( std::string const & path ) {

    this->path = path;
}
std::string Request::getPath( void ) const {

    return (this->path);
}

std::string Request::getHeader( void ) const {

    return (this->header);
}
void Request::setHeader( std::string const & header ) {

    this->header = header;
}

std::string Request::getBody( void ) const {

    return (this->body);
}
void Request::setBody( std::string const & body ) {

    this->body = body;
}

std::string Request::getConnection( void ) const {

    return (this->connection);
}
void Request::setConnection( std::string const & connection ) {

    this->connection = connection;
}

size_t Request::getRequestBodySize( void ) const {

    std::string subString = "\r\n\r\n";

    size_t found = recString.find(subString);
    if (found != std::string::npos) {
        return (recString.length() - (found + subString.length()));
    }
    else
        return (0);
}

std::string Request::getRangeStart( void ) const {

    return (this->rangeStart);
}
void Request::setRangeStart( std::string const & rangeStart ) {
    this->rangeStart = rangeStart;
}
std::string Request::getRangeEnd( void ) const {

    return (this->rangeEnd);
}
void Request::setRangeEnd( std::string const & rangeEnd ) {

    this->rangeEnd = rangeEnd;
}

size_t Request::getRangeStartNum( void ) const {

    return (this->rangeStartNum);
}
void Request::setRangeStartNum( size_t const & rangeStartNum ) {

    this->rangeStartNum = rangeStartNum;
}
size_t Request::getRangeEndNum( void ) const {

    return (this->rangeEndNum);
}
void Request::setRangeEndNum( size_t const & rangeEndNum ) {

    this->rangeEndNum = rangeEndNum;
}

void Request::setHttpVersion ( std::string const & httpVersion ) {

    this->httpVersion = httpVersion;
}
std::string Request::getHttpVersion( void ) const {

    return (this->httpVersion);
}

int Request::setRequestHeader( void ) {

    std::string subString = "\r\n\r\n";

    size_t found = recString.find(subString);
    if (found != std::string::npos) {
        this->header = recString.substr(0, found + subString.length());
        // std::cout << "header request: " << this->header << std::endl;
        return (1);
    }
    else
        return (0);
}
void Request::setRequestBody( void ) {

    std::string subString = "\r\n\r\n";

    size_t found = recString.find(subString);
    this->body = recString.substr(found + subString.length());
    // std::cout << "body request: " << this->body << std::endl;
}

int Request::parseRequestHeader( Conf::Server & server, Response & response ) {

	(void)server;
    std::string requestLine;
	std::istringstream recbuffStream(recString);

    std::string host = getValue("host:");
    /*   get server !!!!!!!!   */

    //get request line "GET / HTTP/1.1"
	std::getline(recbuffStream, requestLine);

    //parse request line
	std::istringstream methodStream(requestLine);

	std::getline(methodStream, this->method, ' ');
    if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        return (0);
    // check method is valid !!!!!!

	std::getline(methodStream, this->path, ' ');

    std::getline(methodStream, this->httpVersion, '\r');
    if (this->httpVersion != "HTTP/1.1") {
        response.setStatusCode( 505 );
        return (0);
    }

	this->path.erase(0, 1);
    if (path.empty()) {
        path = getIndex(server.getIndex().getIndexes(), server.getRoot().getPath());
        // std::cout << "path: " << path << std::endl;
        if (path.empty()) {
                std::cout << "here: " << server.getAutoIndex().getToggle() << std::endl;
            if (!server.getAutoIndex().getToggle()) {
                response.setStatusCode( 403 );
                return (0);
            }
            else {
                response.setAutoIndexing( true );
            }
        }
    }
    else {
        this->path = server.getRoot().getPath() + "/" + this->path;
        // std::cout << "path: " << this->path << std::endl;
        if (access(this->path.c_str(), F_OK) == -1) {
            response.setStatusCode( 404 );
            return (0);
        }
    }

    this->connection = getValue("Connection:");
    response.setContentLength( get_size_fd(this->path) );

    //get Range
	std::string range;
	while (std::getline(recbuffStream, range)) {
		if (range.substr(0, 13) == "Range: bytes=") {
			int numS = range.find('-') - (range.find('=') + 1);
			this->rangeStart = range.substr(range.find('=') + 1, numS);
			this->rangeStartNum = stringToInt(this->rangeStart);
			if (range.find('-') + 1 < range.length() - 1 )
				this->rangeEnd = range.substr(range.find('-') + 1);
			if (this->rangeEnd.empty()) {
				this->rangeEndNum = get_size_fd(this->path) - 1;
				this->rangeEnd = intToString(this->rangeEndNum);
			}
			else
				this->rangeEndNum = stringToInt(this->rangeEnd);
			break;
		}
	}
    return (1);
}

std::string Request::getValue( std::string const & key ) const {

    std::string line;
    std::istringstream recbuffStream(recString);

    while (std::getline(recbuffStream, line))
    {
        std::string part;
        std::istringstream lineStream(line);
        std::getline(lineStream, part, ' ');
        if (part == key) {
            std::getline(lineStream, part);
            return (part);
        }
    }
    return ("");
}

std::string Request::getIndex( std::vector<std::string> const & indexes, std::string const & root ) const {

    std::string fullIndex;
    for (int i = 0; i < (int)indexes.size(); i++) {
        fullIndex = root + "/" + indexes[i];
        if (!access(fullIndex.c_str(), F_OK)) {
            return (fullIndex);
        }
    }
    return ("");
}
