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

    // size_t maxSize = server.getClientMaxBodySize().getSize();
    char recievebuff[SIZE];
    int recieved = recv( sockfd, recievebuff, SIZE, 0 );
    if ( recieved <= 0 ) {

        if ( recieved == 0 ) {
            return (0); // end recieve request
        }
        this->response.setStausCode( 500 );
        return (0); // error
    }
    else {

        recievebuff[recieved] = '\0';
        this->request.setRecString( std::string(recievebuff, recieved) );
        if (this->request.setRequestHeader()) {
            this->request.parseRequestHeader( this->server );
        }
        // if (this->request.getRequestBodySize() > maxSize) {
        //     this->response.setStausCode( 413 );
        //     return (0); // error
        // }
    }
    return (1); // still read request
}