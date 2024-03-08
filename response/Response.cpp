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
    this->countBytesRead = 0;
    this->contentResponse = 0;
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

void Response::setCountBytesRead( size_t const & countBytesRead ) {

    this->countBytesRead = countBytesRead;
}
size_t Response::getCountBytesRead( void ) const {

    return (this->countBytesRead);
}

void Response::setContentResponse( size_t const & contentResponse ) {

    this->contentResponse = contentResponse;
}
size_t Response::getContentResponse( void ) const {

    return (this->contentResponse);
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
    statusMessages[501] = "Not Implemented"; // The request method is not supported
    statusMessages[505] = "HTTP Version Not Supported";

    std::map<int, std::string>::iterator it = statusMessages.find(statusCode);
    if (it != statusMessages.end()) {
        return it->second;
    } else {
        return "Unknown Status Code";
    }
}

size_t Response::sendHeader( int const &sockfd, Request const & request ) {

    // ---------type && mime type-------- //
    std::string type = request.getPath().substr(request.getPath().rfind('.') + 1);
    std::string mimeType = getMimeType(type);

    // ----------status line----------- //
    std::string statusLine;
    if (access(request.getPath().c_str(), F_OK) == -1) {
        this->statusCode = 404 ;
        this->notFound = true;
    }
    else {
        this->statusCode = 200;
        if (!request.getRangeStart().empty())
            this->statusCode = 206;
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

    size_t sended;
    sended = send( sockfd, ( headerResponse.c_str() ), headerResponse.length(), 0 );
    return (sended);
}

size_t Response::sendBody( int const &sockfd, Request const & request ) {

    // -----------open file---------- //
    if (this->file == -2) {
        this->file = open( request.getPath().c_str(), O_RDONLY, 0777 );
        if ( this->file == -1 ) {
            std::cerr << "failed to open file" << std::endl;
            return (-1); // to remove client and poll
        }
    }

    char buffer[SEND];
    char bufferS[1000000];
    size_t sended;

    // --------seek the file-------- //
    if ( !request.getRangeStart().empty() && this->countBytesRead < request.getRangeStartNum() ) {
        size_t bufferSize = 1000000;
        if (request.getRangeStartNum() - this->countBytesRead < 1000000)
            bufferSize = request.getRangeStartNum() - this->countBytesRead;
        size_t bytesRead = read(this->file, bufferS, bufferSize);
        this->countBytesRead += bytesRead;
    }
    else { // --------read file to send----------//
        size_t bytesRead = read(this->file, buffer, SEND);
        if (bytesRead < SEND)
            buffer[bytesRead] = '\0';
        this->contentResponse += bytesRead;
        std::string message = std::string(buffer, bytesRead);
        sended = send( sockfd, ( message.c_str() ), message.length(), 0 );
        return (sended);
    }
    return (1);
}