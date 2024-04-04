/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:42 by del-yaag          #+#    #+#             */
/*   Updated: 2024/04/04 03:11:20 by del-yaag         ###   ########.fr       */
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
		this->defaultServer = obj.defaultServer;
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

Conf::Server const &Client::getDefaultServer( void ) const {
	
	return this->defaultServer;
}

void Client::setDefaultServer( Conf::Server const &server ) {
	
	this->defaultServer = server;
}

Response &Client::getResponse( void ) {

	return this->response;
}

Request Client::getRequest( void ) const {

	return this->request;
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

		this->settimeout( std::time( NULL ) ); // set time out of client
		if (recieved < SIZE)
			recievebuff[recieved] = '\0';
		this->request.setRecString( std::string(recievebuff, recieved) );

		if (!this->endRecHeader) {
			
			if (this->request.setRequestHeader()) {
				this->request.setCgiFiles(intToString(this->sockfd));
				if ( !this->request.parseRequestHeader(this->config, this->server, this->defaultServer, this->response)) {
					return (0); // error
				}
				
				this->endRecHeader = true;
				if ( this->request.getMethod() == "GET" || this->request.getMethod() == "DELETE")
					return 0;
				
				// parse post body ( if body is too small )
				this->request.setRequestBody();
				status = this->request.parsePostBody( this->request.getBody() );
				// std::cout << this->request.getBody() << std::endl;
				if ( status == 2 ) {

					this->response.setStatusCode( 501 );
					return 0;
					
				} else if ( !status ) {

					this->response.setBody( this->request.getBody() );
					return 0; // end recive
				} else if ( status == 3 ) {

					this->response.setStatusCode( 411 );
					return 0; // error Content-Length required
				}
			}
   
			// invalid header *error*
			// if (recieved < SIZE && this->request.getHeader().empty()) {
				
			//     std::cout << "hello" << std::endl;
			//     this->response.setStatusCode( 400 );
			//     return (0); // error
			// }

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
			
			status = this->request.parsePostBody( buffer );
			if ( status == 2 ) {


				this->response.setStatusCode( 501 );
				return 0;
				
			} else if ( !status ) {
				
				this->response.setBody( this->request.getBody() );
				return 0; // end recive
			} else if ( status == 3 ) {
			
				this->response.setStatusCode( 411 );
				return 0; // error Content-Length required
			}
		}
	}
	return (1); // still read request
}

int Client::checkErrorCases( void ) {
	
	ssize_t sended = this->response.sendBody( this->sockfd, this->request );
	if ( (int)sended == -1 ) {

		close(this->response.getFile());
		if (this->request.isCgi())
			remove(this->request.getCgi().getCgiOutFile().c_str());
		this->response.setStatusCode( 500 );
		return 1;
	}
	if ( (response.getContentResponse() == response.getContentLength() && request.getConnection() == "close")) {
		close(this->response.getFile());
		if (this->request.isCgi())
			remove(this->request.getCgi().getCgiOutFile().c_str());
		return (0);
	}
	if (response.getContentResponse() == response.getContentLength()) {
		close(this->response.getFile());
		if (this->request.isCgi())
			remove(this->request.getCgi().getCgiOutFile().c_str());
		return (2); // change to PULLIN
	}
	return (1);
}

int Client::sendresponse() {

	// display error
	if (response.getStatusCode() >= 400)
		return (response.displayErrorPage(this->server, this->sockfd, this->request));

	// display autoIndex
	if (response.getAutoIndexing()) {
		if ( !response.displayAutoIndex(this->server, sockfd, this->request) ) {
			return (1);
		}
		if (this->request.getConnection() == "close")
			return (0);
		return (2); // change to PULLIN
	}

	//	Cgi
	if (this->request.isCgi() && this->request.getCgi().ready() && !this->response.getSendedHeader())
	{
		if (!this->request.getCgi().isStarted())
			this->request.getCgi().launch(response.getSessionId(), request.getCookie());
		return (monitorCgiProcess(request, response, sockfd));
	}

	if (this->request.getMethod() == "GET") {

		if (this->response.getSendedHeader()) {
			// ssize_t sended = this->response.sendBody( this->sockfd, this->request );
			// if ((int)sended == -1 || (response.getContentResponse() == response.getContentLength() && request.getConnection() == "close")) {
			// 	close(this->response.getFile());
			// 	if (this->request.isCgi())
			// 		remove(this->request.getCgi().getCgiOutFile().c_str());
			//     return (0);
			// }
			// if (response.getContentResponse() == response.getContentLength()) {
			// 	close(this->response.getFile());
			// 	if (this->request.isCgi())
			// 		remove(this->request.getCgi().getCgiOutFile().c_str());
			//     return (2); // change to PULLIN
			// }
			return this->checkErrorCases();
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

		int status = -1;
	   	if (!this->response.getSendedHeader())
			status = this->response.execPostMethod( this->request, this->server );

		if (this->request.isCgi())
		{
			if (this->response.getSendedHeader()) {
				// ssize_t sended = this->response.sendBody( this->sockfd, this->request );
				// if ((int)sended == -1 || (response.getContentResponse() == response.getContentLength() && request.getConnection() == "close")) {
				// 	close(this->response.getFile());
				// 	if (this->request.isCgi())
				// 		remove(this->request.getCgi().getCgiOutFile().c_str());
				//     return (0);
				// }
				// if (response.getContentResponse() == response.getContentLength()) {
				// 	close(this->response.getFile());
				// 	if (this->request.isCgi())
				// 		remove(this->request.getCgi().getCgiOutFile().c_str());
				//     return (2); // change to PULLIN
				// }
				return this->checkErrorCases();
			}
			return (1);
		}

		if ( !status ) {
			
			if ( !this->sendPostResponse( "data uploaded successfully" ) )
				return 1;
			std::cout << BLUE << "\tPOST done." << std::endl << std::endl;
			return 0;
		} else if ( status == 2 ) {
			
			if ( !this->sendPostResponse( "data uploaded successfully" ) )
				return 1;
			std::cout << BLUE << "\tPOST done." << std::endl << std::endl;
			return 2;
		} else if ( status == 3 ) {
			
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
	
	body = "<!DOCTYPE html>\n"
				"<html lang=\"en\">\n"
				"<head>\n"
				"    <title>Document</title>\n"
				"    <style>\n"
				"        :root{ --accent-100:#3D5A80; --text-100:#FFFFFF; --bg-100:#0F1C2E; }\n"
				"        *{ padding: 0; margin: 0; box-sizing: border-box; font-family: Arial, sans-serif; }\n"
				"        body{ display: flex; justify-content: center; align-items: center; height: 100vh; background-color: var(--bg-100); }\n"
				"        .container{ width: 700px; padding: 20px; text-align: center; }\n"
				"        .container h1{ color: var(--text-100); margin-bottom: 15px; }\n"
				"        .container p{ font-size: 20px; color: var(--accent-100); }\n"
				"    </style>\n"
				"</head>\n"
				"<body>\n"
				"    <div class=\"container\">\n"
				"        <h1>" + message + ".</h1>\n"
				"        <p>all data was uploaded successfully.</p>\n"
				"    </div>\n"
				"</body>\n"
				"</html>\n";
	header = "HTTP/1.1 " + intToString( this->response.getStatusCode() ) + " " + this->response.getStatusMessage( this->response.getStatusCode() ) + "\r\n";
	header += "Content-Type: text/html\r\n";
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
