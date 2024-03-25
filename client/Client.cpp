/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:42 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/25 15:37:48 by mmisskin         ###   ########.fr       */
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

Client::Client(Client const & src) {

    *this = src;
}
Client	&Client::operator=(Client const & obj) {

    if (this != &obj) {
        this->sockfd = obj.sockfd;
        this->timeout = obj.timeout;
        this->server = obj.server;
        this->config = obj.config;
    }
    return (*this);
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

int Client::recieveRequest() {

    char recievebuff[SIZE];
    int recieved = recv( this->sockfd, recievebuff, SIZE, 0 );
    if ( recieved < 0 ) {
        this->response.setStatusCode( 500 );
        return (0); // error
    }
    else {

        if (recieved < SIZE)
            recievebuff[recieved] = '\0';
        this->request.setRecString( std::string(recievebuff, recieved) );
        if (!this->endRecHeader) {
            if (this->request.setRequestHeader()) {
                if ( !this->request.parseRequestHeader(this->config, this->server, this->response , this->sockfd)) {
                    return (0); // error
                }
                this->endRecHeader = true;
                this->request.setRequestBody();
                if (request.getValue("Content-Length:").empty())
                    return (0); // end recieve request
            }
            if (recieved < SEND && this->request.getHeader().empty()) {
                this->response.setStatusCode( 400 );
                return (0); // error
            }
        }
        else {
            size_t maxSize = server.getClientMaxBodySize().getSize();
            if (this->request.getCheckLocation())
                maxSize = request.getLocation().getClientMaxBodySize().getSize();
            if (this->request.getRequestBodySize() > maxSize) {
                this->response.setStatusCode( 413 );
                return (0); // error
            }
            if (this->request.getRequestBodySize() >= stringToInt(request.getValue("Content-Length:"))) {
                this->request.setRequestBody();
                std::cout << "body : " << request.getBody() << std::endl;
                return (0);
            }
        }
    }
    if (!request.getBody().empty() && this->request.getRequestBodySize() >= stringToInt(request.getValue("Content-Length:"))) {
        return (0); // end recieve request
    }
    return (1); // still read request
}

int Client::sendresponse() {

    if (response.getStatusCode() >= 400) {
        response.displayErrorPage(this->server, this->sockfd, this->request);
        return (0);
    }
    if (response.getAutoIndexing()) {
        if ( !response.displayAutoIndex(this->server, this->sockfd, this->request) ) {
            response.displayErrorPage(this->server, this->sockfd, this->request);
            return (0);
        }
        return (1);
    }

	if (this->request.getMethod() == "GET") {
		if (this->request.cgi)
		{
			/* check if the script has finished */
			if (this->request.pid == waitpid(this->request.pid, NULL, WNOHANG))
			{
				std::cout << "Cgi finished" << std::endl;
				if (response.sendCgiHeader(this->sockfd, this->request) == -1)
					return (0);
				else
					response.setSendedHeader( true );
				this->request.cgi = false;
			}
			else
				return (1);
			/* if it finished send the response */
			/* otherwise if the timeout occurs kill it */
			/* if non of the above return 1 */
		}
        if (this->response.getSendedHeader()) {
            ssize_t sended = this->response.sendBody( this->sockfd, this->request );
            if ((int)sended == -1 || (response.getContentResponse() == response.getContentLength() && request.getConnection() == "close")) {
				close(this->response.getFile());
                return (0);
            }
            if (response.getContentResponse() == response.getContentLength()) {
				close(this->response.getFile());
                return (2); // change to PULLIN
            }
        }
        else {
            ssize_t sended = this->response.sendHeader( this->sockfd, this->request );
            if ( (int)sended == -1) {
                return (0);
            }
            else
                response.setSendedHeader( true );
        }
    }
    else if (this->request.getMethod() == "POST") {
        
    }
    return (1);
}
