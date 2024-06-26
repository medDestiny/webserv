/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 15:19:45 by del-yaag          #+#    #+#             */
/*   Updated: 2024/04/04 03:10:59 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../response/Response.hpp"
		
std::string Request::getStartBoundary( void ) const {
	
	return this->startBoundary;
}
void Request::setStartBoundary( std::string const &boundary ) {
	
	this->startBoundary = boundary;
}

std::string Request::getEndBoundary( void ) const {
	
	return this->endBoundary;
}

void Request::setEndBoundary( std::string const &boundary ) {
	
	this->endBoundary = boundary;
}

std::string Request::getContentType( void ) const {
	
	return this->contentType;
}

void Request::setContentType( std::string const &type ) {
	
	this->contentType = type;
}

size_t Request::getContentLength( void ) const {
	
	return this->contentLength;
}

void Request::setContentLength( size_t const &length ) {
	
	this->contentLength = length;
}

void Request::setPostBodyHeader( std::string const &header ) {
	
	this->postBodyHeader = header;
}

std::string Request::getPostBodyHeader( void ) const {
	
	return this->postBodyHeader;
}

int Request::getBodyType( void ) const {
	
	return this->bodyType;
}

void Request::setBodyType( int const &type ) {
	
	this->bodyType = type;
}

std::string Request::getTransferEncoding( void ) const {
	
	return this->transferEncoding;
}

void Request::setTransferEncoding( std::string const &content ) {
	
	this->transferEncoding = content;
}

void Request::parsePostHeader( void ) {
	
	size_t length;
	std::string str;

	str = this->getValue( "Content-Type:" ); // get content type and boundaries
	if ( str.find( " boundary=" ) != std::string::npos ) {
		
		this->setContentType( str.substr( 0, str.find( " boundary=" ) ) );
		this->setStartBoundary( str.substr( str.find( " boundary=" ) + std::strlen( " boundary=" ) ).insert( 0, 2, '-' ) );
		this->setEndBoundary( this->getStartBoundary().append( "--" ) );
	}
	std::istringstream iss( this->getValue( "Content-Length:" ) ); // get content length
	if ( iss >> length && iss.peek() == EOF )
		this->setContentLength( length );
	else
		this->setContentLength( 0 );
	this->setTransferEncoding( this->getValue( "Transfer-Encoding:" ) ); // get transfer encoding
}

int Request::bufferPostBody( std::string const &buffer ) {
	
	size_t size = this->body.size();
	std::string body;
	if ( size > 200 )
		body = this->body.substr( size - 200, 200 );
	if ( this->getContentType() != "multipart/form-data;" && !isCgi() )
		return 2;
	if ( !this->getTransferEncoding().empty() ) {
		
		if ( this->getTransferEncoding().find( "chunked" ) != std::string::npos ) {
			
			if ( buffer.find( "0\r\n\r\n" ) != std::string::npos ) { // encoding
				
				this->setBodyType( ENCODING );
				return 0;
			} else if ( body.find( "0\r\n\r\n" ) != std::string::npos ) { // encoding
				
				this->setBodyType( ENCODING );
				return 0;
			}
		} else
			return 2;
	} else if ( !this->getEndBoundary().empty() ) {

		if ( buffer.find( this->getEndBoundary() ) != std::string::npos ) { // boundaries

			this->setBodyType( BOUNDARIES );
			return 0;
		} else if ( body.find( this->getEndBoundary() ) != std::string::npos ) {
			
			this->setBodyType( BOUNDARIES );
			return 0;
		}
	} else if ( this->getContentLength() ) {

		if ( this->getRequestBodySize() == this->getContentLength() ) { // length

			this->setBodyType( LENGTH );
			return 0;
		}
		
	} else if ( !this->getContentLength() ) {

		if ( isCgi() )
			return 3;
		return 2;
	}
	return 1;
}

int Request::parsePostBody( std::string const &buffer ) {
	
	int status = this->bufferPostBody( buffer );
	
	if ( !status ) {

		if ( this->getBodyType() == ENCODING )
			return 0;
		else if ( this->getBodyType() == BOUNDARIES )
			return 0;
		else if ( this->getBodyType() == LENGTH )
			return 0;
	}
	return status;
}
