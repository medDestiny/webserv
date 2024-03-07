/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:29 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:54:30 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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

void Request::setRecString( std::string  const & recString ) {

    this->recString += recString;
}
std::string Request::getRecString( void ) const {

    return (this->recString);
}

void Request::setMethod( std::string  const & method ) {

    this->method = method;
}
std::string Request::getMethod( void ) const {

    return (this->method);
}

void Request::setPath( std::string  const & path ) {

    this->path = path;
}
std::string Request::getPath( void ) const {

    return (this->path);
}

std::string Request::getHeader( void ) const {

    return (this->header);
}
void Request::setHeader( std::string  const & header ) {

    this->header = header;
}

std::string Request::getBody( void ) const {

    return (this->body);
}
void Request::setBody( std::string  const & body ) {

    this->body = body;
}

std::string Request::getConnection( void ) const {

    return (this->connection);
}
void Request::setConnection( std::string const & connection ) {

    this->connection = connection;
}

int Request::getRequestBodySize( void ) const {

    std::string subString = "\r\n\r\n";

    size_t found = recString.find(subString);
    if (found != std::string::npos) {
        return (recString.length() - (found + subString.length()));
    }
    else
        return (0);
}

void Request::parseRequest( void ) {

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

void Request::parseRequestHeader( Conf::Server & server ) {

    std::string requestLine;
	std::istringstream recbuffStream(recString);

    /*   get server   */

    std::string host = getValue("host:");
    //get request line "GET / HTTP/1.1"
	std::getline(recbuffStream, requestLine);

    //parse request line
    std::string httpVersion;
	std::istringstream methodStream(requestLine);

	std::getline(methodStream, method, ' ');
    // check method is valid

	std::getline(methodStream, path, ' ');

    std::getline(methodStream, httpVersion);

	path.erase(0, 1);
    if (path.empty()) {
        
    }

    this->connection = getValue("Connection:");
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