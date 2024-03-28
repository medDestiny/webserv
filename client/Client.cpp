/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:42 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/28 22:25:43 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include<signal.h>

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

    int status;
    std::string buffer;
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
				this->request.setCgiFileSuffix(intToString(this->sockfd));
                if ( !this->request.parseRequestHeader(this->config, this->server, this->response)) {
                    return (0); // error
                }
                
                this->endRecHeader = true;
                if ( this->request.getMethod() == "GET" || this->request.getMethod() == "DELETE" )
                    return 0;
                
                // parse post body ( if body is too small )
                this->request.setRequestBody();
                status = this->request.parsePostBody( this->request.getBody() );
                if ( status == 2 ) {

                    this->response.setStatusCode( 501 );
                    return 0;
                    
                } else if ( !status ) {

                    this->response.setBody( this->request.getBody() );
                    return 0; // end recive
                }
            }
            
            // invalid header *error*
            if (recieved < SENDED && this->request.getHeader().empty()) {
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
            
            this->request.setBody( recievebuff, recieved );
            buffer = std::string( recievebuff, recieved );
            this->settimeout( std::time( NULL ) );
            
            status = this->request.parsePostBody( buffer );
            
            if ( status == 2 ) {

                this->response.setStatusCode( 400 );
                return 0;
                
            } else if ( !status ) {

                this->response.setBody( this->request.getBody() );
                return 0; // end recive
            } 
        }
    }
    return (1); // still read request
}

int Client::sendresponse() {

    // display error
    if (response.getStatusCode() >= 400) {
        response.displayErrorPage(this->server, this->sockfd, this->request);
        return (0);
    }

    // display autoIndex
    if (response.getAutoIndexing()) {
        if ( !response.displayAutoIndex(this->server, sockfd, this->request) ) {
            return (1);
        }
        return (2); // change to PULLIN
    }

	if (this->request.getMethod() == "GET") {

		if (this->request.isCgi() && !this->response.getSendedHeader())
		{
			/* 
			 * check if the script has finished
			 * if it finished send the response header
			 * otherwise if the timeout or an error occurs kill it
			 * if none of the above return 1
			 */
			Cgi		cgi = this->request.getCgi();
			char	tmp[1025] = {0};
			int		err = read(cgi.getCgiStdErr(), tmp, 1024);
			if (cgi.getPid() == waitpid(cgi.getPid(), NULL, WNOHANG))
			{
				std::cout << "Cgi finished" << std::endl;
				close(cgi.getCgiStdErr());
				if (response.sendCgiHeader(this->sockfd, this->request) == -1)
					return (0);
				else
					response.setSendedHeader( true );
			}
			else if (std::time(NULL) - cgi.getCgiTime() >= 30
					|| err > 0)
			{
				std::cout << RED << "stderr: " << err << " \'" << tmp << "\'"<< RESET << std::endl;
				std::cout << "killed" << std::endl;
				close(cgi.getCgiStdErr());
				kill(cgi.getPid(), SIGKILL);
				remove(cgi.getCgiTmpFile().c_str());

				if (err > 0)
					response.setStatusCode(500);
				else
					response.setStatusCode(504);
			}
			return (1);
		}
        if (this->response.getSendedHeader()) {
            ssize_t sended = this->response.sendBody( this->sockfd, this->request );
            if ((int)sended == -1 || (response.getContentResponse() == response.getContentLength() && request.getConnection() == "close")) {
				close(this->response.getFile());
				if (this->request.getCgi().isSet())
					remove(this->request.getCgi().getCgiTmpFile().c_str());
                return (0);
            }
            if (response.getContentResponse() == response.getContentLength()) {
				close(this->response.getFile());
				if (this->request.getCgi().isSet())
					remove(this->request.getCgi().getCgiTmpFile().c_str());
                return (2); // change to PULLIN
            }
        }
        else {
            ssize_t sended = this->response.sendHeader( this->sockfd, this->request );
            if ( (int)sended == -1) {
                response.setStatusCode( 500 );
                return (1); // remove client and fd
            }
            else
                response.setSendedHeader( true );
        }
    }
    else if (this->request.getMethod() == "POST") {
        
        int status = this->response.execPostMethod( this->request, this->server );
        if ( !status ) {
            
            if ( !this->sendPostResponse( "data uploaded successfully" ) )
                return 1;
            std::cout << BLUE << "\tPOST done." << std::endl << std::endl;
            return 0;
        }
        else if ( status == 2 ) {
            
            if ( !this->sendPostResponse( "data uploaded successfully" ) )
                return 1;
            std::cout << BLUE << "\tPOST done." << std::endl << std::endl;
            return 2;
        } else if ( status == 3 ) {

            if ( !this->sendPostResponse( "all good but cgi not implemented yet" ) )
                return 1;
            std::cout << BLUE << "\tPOST done." << RESET << std::endl;
            return 0;
        }
        else
            this->settimeout( std::time( NULL ) );
    }
    else if (this->request.getMethod() == "DELETE") {
        if ( !this->response.deleteResource(this->sockfd, this->request) )
            return (1); // error
        else
            return (2); // change to PULLIN
    }
    return (1);
}

int Client::sendPostResponse( std::string const &message ) {

    ssize_t bytes;
    std::string header;
    std::string body;
    
    body = "<!DOCTYPE html>\n";
    body += "<html>\n";
    body += "<head><title> POST METHOD </title>\n";
    body += "<style> * { background-color: #1E1E1E; font-family: Fantasy; color: #2E8B57; } </style></head>\n";
    body += "<body>\n";
    body += "<center><h1>" + message + "</h1></center>\n";
    body += "</body>\n";
    body += "</html>";
    
    header = "HTTP/1.1 " + intToString( this->response.getStatusCode() ) + " " + this->response.getStatusMessage( this->response.getStatusCode() ) + "\r\n";
    header += "Content-Type: text/plain\r\n";
    header += "Content-Length: " + intToString( body.size() ) + "\r\n";
    header += "Connection: " + request.getConnection() + "\r\n\r\n";
    header += body;
    
    bytes = send( this->sockfd, header.c_str(), header.size(), 0 ); // send response
    if ( bytes == -1 ) {

        this->response.setStatusCode( 500 );
        return 0;
    }
    return 1;
}