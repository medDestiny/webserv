/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 15:19:45 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/19 21:10:06 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
        
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

std::string Request::getBHName( void ) const {
    
    return this->BHName;
}

void Request::setBHName( std::string const &name ) {
    
    this->BHName = name;
}

std::string Request::getBHFilename( void ) const {
    
    return this->BHFilename;
}

void Request::setBHFilename( std::string const &filename ) {
    
    this->BHFilename = filename;
}

std::string Request::getBHContentDispo( void ) const {
    
    return this->BHContentDispo;
}

void Request::setBHContentDispo( std::string const &content ) {
    
    this->BHContentDispo = content;
}

std::string Request::getBHContentType( void ) const {
    
    return this->BHContentType;
}

void Request::setBHContentType( std::string const &type ) {
    
    this->BHContentType = type;
}

std::string Request::getTransferEncoding( void ) const {
    
    return this->transferEncoding;
}

void Request::setTransferEncoding( std::string const &content ) {
    
    this->transferEncoding = content;
}

int Request::getBodyType( void ) const {
    
    return this->bodyType;
}

void Request::setBodyType( int const &type ) {
    
    this->bodyType = type;
}

void Request::parsePostHeader( void ) {
    
    size_t length;
    std::string str;

    // get content type and boundaries
    str = this->getValue( "Content-Type:" );
    if ( str.find( " boundary=" ) != std::string::npos ) {
        
        this->setContentType( str.substr( 0, str.find( " boundary=" ) ) );
        this->setStartBoundary( str.substr( str.find( " boundary=" ) + std::strlen( " boundary=" ) ) );
        this->setEndBoundary( this->getStartBoundary().append( "--" ) );
    }
    
    // get content length
    std::istringstream iss( this->getValue( "Content-Length:" ) );
    if ( iss >> length && iss.peek() == EOF )
        this->setContentLength( length );
    else
        this->setContentLength( 0 );
    
    // get transfer encoding
    this->setTransferEncoding( this->getValue( "Transfer-Encoding:" ) );
}

void Request::parsePostBodyHeader( std::string const &chunck ) {

    size_t boundary;
    size_t endHeader;
    
    boundary = chunck.find( this->getStartBoundary() );
    if ( boundary != std::string::npos ) {
        
        endHeader = chunck.find( "\r\n\r\n" );
        if ( endHeader != std::string::npos ) {
            
            this->setPostBodyHeader( chunck.substr( boundary, endHeader - boundary ) );
            std::cout << RED << "|" << this->getPostBodyHeader() << "|" << RESET << std::endl;
        } else {
            
            this->setPostBodyHeader( chunck.substr( boundary ) );
            std::cout << BLUE << "|" << this->getPostBodyHeader() << "|" << RESET << std::endl;
        }
    }
}

int Request::bufferPostBody( void ) {
    
    if ( this->getContentLength() == 0 ) {
                
        if ( this->getEndBoundary().empty() ) {
            
            if ( this->getTransferEncoding().empty() || this->getTransferEncoding() != "chunked") // bad request we can't know how to stop reading body
                return 2;
            else {

                if ( this->getBody().find( "0\r\n\r\n" ) == std::string::npos ) { // encoding
                    
                    this->setBodyType( ENCODING );
                    return 0;
                }
            }
        }
        else {
            
            if ( this->getTransferEncoding().empty() || this->getTransferEncoding() != "chunked" ) {

                if ( this->getBody().find( this->getEndBoundary() ) != std::string::npos ) { // boundries

                    this->setBodyType( BOUNDARIES );
                    return 0;
                }
            } else {

                if ( this->getBody().find( "0\r\n\r\n" ) != std::string::npos ) { // boundaries width encoding
                    
                    this->setBodyType( ENCODING );
                    return 0;
                }
            }
        }
        
    } else if ( this->getRequestBodySize() >= this->getContentLength() ) { // if there is a content length
        
        if ( this->getTransferEncoding().empty() || this->getTransferEncoding() != "chunked" ) {

            if ( this->getRequestBodySize() == this->getContentLength() ) { // length

                this->setBodyType( LENGTH );
                return 0;
            }
        } else {

            if ( this->getBody().find( "0\r\n\r\n" ) != std::string::npos ) { // boundaries width encoding
                
                this->setBodyType( ENCODING );
                return 0;
            }
        }
    }
    
    return 1;
}