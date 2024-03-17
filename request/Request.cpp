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
    this->checkLocation = false;
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

    if (this->body.empty())
        this->body = body;
    else
        this->body += body;
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

std::map<std::string, std::string> const & Request::getLinesRequest( void ) const {

    return (this->linesRequest);
}

Location const & Request::getLocation( void ) const {

    return (this->location);
}
void Request::setLocation( Location const & location ) {

    this->location = location;
}
bool Request::getCheckLocation( void ) const {

    return (this->checkLocation);
}
void Request::setCheckLocation( bool checkLocation ) {

    this->checkLocation = checkLocation;
}

std::string Request::getStringLocation( void ) const {

    return (this->stringLocation);
}
void Request::setStringLocation( std::string const & stringLocation ) {

    this->stringLocation = stringLocation;
}

std::string Request::getUrl( void ) const {

    return (this->url);
}
void Request::setUrl( std::string url ) {

    this->url = url;
}

int Request::setRequestHeader( void ) {

    std::string subString = "\r\n\r\n";

    size_t found = recString.find(subString);
    if (found != std::string::npos) {
        this->header = recString.substr(0, found + subString.length());
        std::cout << "header request: " << this->header << std::endl;
        return (1);
    }
    else
        return (0);
}
void Request::setRequestBody( void ) {

    std::string subString = "\r\n\r\n";

    size_t found = recString.find(subString);
    this->body = recString.substr(found + subString.length());

}

int Request::parseRequestHeader( Config conf, Conf::Server & server, Response & response ) {

    std::string requestLine;
	std::istringstream headerStream(this->header);

    std::string host = getValue("Host:");
    server = conf.getServer(server, host);

    //get request line "GET / HTTP/1.1"
	std::getline(headerStream, requestLine);

    //parse request line
	std::istringstream methodStream(requestLine);

    // get method | path | httpVersion
	std::getline(methodStream, this->method, ' ');
	std::getline(methodStream, this->path, ' ');
    this->url = this->path;
    std::getline(methodStream, this->httpVersion, '\r');
    // check httpVersion is valid
    if (this->httpVersion != "HTTP/1.1") {
        response.setStatusCode( 505 );
        return (0);
    }

    // get location
    // if (this->path[this->path.length() - 1] == '/')
    //     this->path.erase(this->path.length() - 1, 1);
    std::map<std::string, Location>::iterator itLocation = server.getLocation(this->path);
    if (itLocation != server.getLocations().end()) {
        this->location = itLocation->second;
        this->stringLocation = itLocation->first;
        this->checkLocation = true;
    }

    // check method is valid !!!!!!
    if (this->checkLocation && !location.getLimitExcept().empty()) {
        std::set<std::string> setMethods = location.getLimitExcept().getMethods();
        std::set<std::string>::iterator itMethod = setMethods.find(this->method);
        if (itMethod == setMethods.end()) {
            response.setStatusCode( 405 );
            return (0);
        }
    }
    else if (this->method != "GET" && this->method != "POST" && this->method != "DELETE"){
        response.setStatusCode( 405 );
        return (0);
    }

    // get lines of request
    std::string line;
    std::string key;
    std::string value;
    std::istringstream headerStreamtmp(this->header);
    std::getline(headerStreamtmp, line);
    while (std::getline(headerStreamtmp, line) && line != "\r") {
        std::istringstream lineStream(line);
        std::getline(lineStream, key, ' ');
        std::getline(lineStream, value, '\r');
        if (value.empty() || key[key.length() - 1] != ':') {
            response.setStatusCode( 400 );
            return (0);
        }
        this->linesRequest[key] = value;
    }

    // get connection
    std::map<std::string, std::string>::iterator it = this->linesRequest.find("Connection:");
    if (it != this->linesRequest.end())
        this->connection = it->second;
    else
        this->connection = "close";

    if (this->method == "GET") {
        // check path is valid !!!!!
        this->path.erase(0, 1);
        if (this->path.empty() || (isDirectory(this->location.getRoot().getPath() + this->url))) {
            if (this->checkLocation)
                this->path = getIndex(this->location.getIndex().getIndexes(), this->location.getRoot().getPath() + this->stringLocation);
            else
                this->path = getIndex(server.getIndex().getIndexes(), server.getRoot().getPath());
            std::cout << "path: " << path << std::endl;
            if (this->path.empty()) {
                bool checkAutoIndex = server.getAutoIndex().getToggle();
                if (this->checkLocation)
                    checkAutoIndex = this->location.getAutoIndex().getToggle();
                if (!checkAutoIndex) {
                    response.setStatusCode( 403 );
                    return (0);
                }
                else {
                    response.setAutoIndexing( true );
                    return (1);
                }
            }
            // response.setType( this->path.substr(this->path.rfind('.') + 1) );
            // std::cout << "type: " << this->path.substr(this->getPath().rfind('.') + 1) << std::endl;
            // response.setMimeType( getMimeType(response.getType()) );
        }
        else {
            if (this->checkLocation)
                this->path = location.getRoot().getPath() + "/" + this->path;
            else
                this->path = server.getRoot().getPath() + "/" + this->path;
            // std::cout << "path: " << this->path << std::endl;
            if (access(this->path.c_str(), F_OK) == -1) {
                response.setStatusCode( 404 );
                return (0);
            }
            response.setType( this->getPath().substr(this->getPath().rfind('.') + 1) );
            // std::cout << "type: " << this->getPath().substr(this->getPath().rfind('.') + 1) << std::endl;
            response.setMimeType( getMimeType(response.getType()) );
        }

        // get content length
        response.setContentLength( get_size_fd(this->path) );

        //get Range
        std::string range;
        while (std::getline(headerStream, range)) {
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
            std::getline(lineStream, part, '\r');
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
