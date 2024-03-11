/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:42 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:54:44 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) {

    this->endRecHeader = false;
}

Client::~Client( void ) { }

int Client::getsockfd( void ) const {

    return this->sockfd;
}

void Client::setsockfd( int const &sockfd ) {

    this->sockfd = sockfd;
}

std::time_t Client::gettimeout( void ) const {

    return this->timeout;
}

void Client::settimeout( std::time_t const &timeout ) {

    this->timeout = timeout;
}

Conf::Server Client::getserver( void ) const {

    return this->server;
}

void Client::setserver( Conf::Server const &server ) {

    this->server = server;
}

Config Client::getConfig( void ) {

    return (this->config);
}
void Client::setConfig( Config config ) {

    this->config = config;
}

bool Client::getEndRecHeader( void ) {

    return (this->endRecHeader);
}
void Client::setEndRecHeader( bool endRecHeader ) {

    this->endRecHeader = endRecHeader;
}

int Client::recieveRequest( int const &sockfd ) {

    char recievebuff[SIZE];
    int recieved = recv( sockfd, recievebuff, SIZE, 0 );
    if ( recieved <= 0 ) {

        if ( recieved == 0 ) {
            if (this->request.getHeader().empty())
                this->response.setStatusCode( 400 );
            else {
                this->request.setRequestBody();
            }
            return (0); // end recieve request
        }
        this->response.setStatusCode( 500 );
        return (0); // error
    }
    else {

        recievebuff[recieved] = '\0';
        this->request.setRecString( std::string(recievebuff, recieved) );
        if (!this->endRecHeader) {
            if (this->request.setRequestHeader()) {
                if ( !this->request.parseRequestHeader(this->config, this->server, this->response )) {
                    return (0); // error
                }
                if (recieved < SEND)
                    return (0); // end recieve request
                this->endRecHeader = true;
            }
        }
        else {
            size_t maxSize = server.getClientMaxBodySize().getSize();
            if (this->request.getRequestBodySize() > maxSize) {
                this->response.setStatusCode( 413 );
                return (0); // error
            }
        }
    }
    return (1); // still read request
}

int Client::sendresponse( int const &sockfd ) {

    std::cout << "fd: " << sockfd << std::endl;
    if (response.getStatusCode() >= 400) {
        response.displayErrorPage(this->server, sockfd);
        return (0);
    }
    if (response.getAutoIndexing()) {
        if ( !response.displayAutoIndex(this->server, sockfd, this->request) ) {
            response.displayErrorPage(this->server, sockfd);
            return (0);
        }
        return (1);
    }
    if (this->request.getMethod() == "GET") {
        if (this->response.getSendedHeader()) {
            ssize_t sended = this->response.sendBody( sockfd, this->request );
            if ((int)sended == -1 || (response.getContentResponse() == response.getContentLength() && request.getConnection() == "close")) {
                return (0);
            }
            if (response.getContentResponse() == response.getContentLength()) {
                return (2); // change to PULLIN
            }
        }
        else {
            ssize_t sended = this->response.sendHeader( sockfd, this->request );
            if ( (int)sended == -1) {
                return (0);
            }
            else
                response.setSendedHeader( true );
        }
    }
    return (1);
}