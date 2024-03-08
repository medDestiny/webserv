/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:19 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:54:20 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../request/Request.hpp"

Response::Response( void ) {

    this->receivedcontent = 0;
    this->statusCode = 200;
    this->sendedHeader = false;
    this->notFound = false;
    this->file = -2;
}

Response::~Response( void ) { }

int Response::getreceivedcontent( void ) const {

    return this->receivedcontent;
}

void Response::setreceivedcontent( int const &receivedcontent ) {

    this->receivedcontent = receivedcontent;
}

void Response::setStatusCode( int const & statusCode ) {

    this->statusCode = statusCode;
}
int Response::getStatusCode( void ) const {

    return (this->statusCode);
}

void Response::setSendedHeader( bool const & sendedHeader ) {

    this->sendedHeader = sendedHeader;
}
bool Response::getSendedHeader( void ) const {

    return (this->sendedHeader);
}

void Response::setNotFound( bool const & notFound ) {

    this->notFound = notFound;
}
bool Response::getNotFound( void ) const {

    return (this->notFound);
}

void Response::setContentLength( size_t const & contentLength ) {

    this->contentLength = contentLength;
}
size_t Response::getContentLength( void ) const {

    return (this->contentLength);
}

void Response::setFile( int const & file ) {

    this->file = file;
}
int Response::getFile( void ) const {

    return (this->file);
}

std::string Response::getStatusMessage(int const & statusCode) {
    std::map<int, std::string> statusMessages;
    statusMessages[200] = "OK";
    statusMessages[206] = "Partial Content";
    statusMessages[400] = "Bad Request";
    statusMessages[404] = "Not Found";
    statusMessages[413] = "Payload Too Large";
    statusMessages[415] = "Unsupported Media Type";
    statusMessages[500] = "Internal Server Error";

    std::map<int, std::string>::iterator it = statusMessages.find(statusCode);
    if (it != statusMessages.end()) {
        return it->second;
    } else {
        return "Unknown Status Code";
    }
}

void Response::sendHeader( Conf::Server const & server, int const &sockfd, Request const & request ) {

    // ---------type && mime type-------- //
    std::string type = request.getPath().substr(request.getPath().rfind('.') + 1);
    std::string mimeType = getMimeType(type);

    // ----------status line----------- //
    std::string statusLine;
    if (this->statusCode == 200) {
        if (access(request.getPath().c_str(), F_OK) == -1) {
            this->statusCode = 404 ;
            this->notFound = true;
        }
        else {
            this->statusCode = 200;
            if (!request.getRangeStart().empty())
                this->statusCode = 206;
        }
    }
    statusLine = request.getHttpVersion() + " " + std::to_string(this->statusCode) + " " + getStatusMessage(this->statusCode);

    std::string headerResponse;
    headerResponse = statusLine + "\r\nContent-Type: " + mimeType + "\r\nContent-Length: ";
    if (!request.getRangeStart().empty()) {
        headerResponse += std::to_string( request.getRangeEndNum() - request.getRangeStartNum() + 1 ) + "\r\nAccept-Ranges: bytes";
        headerResponse += "\r\nContent-Range: bytes " + request.getRangeStart() + "-" + request.getRangeEnd() + "/" + std::to_string( this->contentLength );
        this->contentLength = request.getRangeEndNum() - request.getRangeStartNum() + 1;
    }
    else
    {
        headerResponse += std::to_string( this->contentLength );
        if (mimeType.substr(0, mimeType.find('/')) == "video" || mimeType.substr(0, mimeType.find('/')) == "audio")
            headerResponse += "\r\nAccept-Ranges: bytes";
    }
    headerResponse += "\r\nConnection: " + request.getConnection();
    headerResponse += "\r\n\r\n";

    ssize_t sended;
    sended = send( server.pfds[sockfd].fd, ( headerResponse.c_str() ), headerResponse.length(), 0 );
    if ( sended == -1 || this->notFound ) {

        perror( "send" );
        std::cout << "aaach had zmar : " << pfds[i].fd << std::endl;
        this->removeclient( itclient );
        this->removepollsock( i );
    }
    else
        this->sendedHeader = true;

}
void Response::sendBody(Conf::Server const & server, int const &sockfd, Request const & request) {

    // -----------open file---------- //
    if (this->file == -2) {
        this->file = open( request.getPath().c_str(), O_RDONLY, 0777 );
        if ( this->file == -1 ) {
            std::cerr << "failed to open file" << std::endl;
        }
    }
}