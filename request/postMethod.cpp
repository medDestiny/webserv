/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 15:19:45 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/23 22:24:39 by del-yaag         ###   ########.fr       */
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

    // get content type and boundaries
    str = this->getValue( "Content-Type:" );
    if ( str.find( " boundary=" ) != std::string::npos ) {
        
        this->setContentType( str.substr( 0, str.find( " boundary=" ) ) );
        this->setStartBoundary( str.substr( str.find( " boundary=" ) + std::strlen( " boundary=" ) ).insert( 0, 2, '-' ) );
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

int Request::bufferPostBody( std::string const &buffer ) {
    
    if ( this->getContentType() != "multipart/form-data;" )
        return 2;
    if ( !this->getTransferEncoding().empty() ) {
        
        if ( this->getTransferEncoding() == "chunked" ) {
            
            // std::cout << " waa3" << std::endl;
            if ( buffer.find( "0\r\n\r\n" ) != std::string::npos ) { // encoding
                
                this->setBodyType( ENCODING );
                return 0;
            }
        } else
            return 2;
    }
    else if ( !this->getEndBoundary().empty() ) {

        if ( buffer.find( this->getEndBoundary() ) != std::string::npos ) { // boundaries

            this->setBodyType( BOUNDARIES );
            return 0;
        }
    }
    else if ( this->getContentLength() ) {

        if ( this->getRequestBodySize() == this->getContentLength() ) { // length

            this->setBodyType( LENGTH );
            return 0;
        }
        
    }
    return 1;
}

// std::ofstream outFile( "/Users/del-yaag/Desktop/" + this->getBHFilename() );
// if ( !outFile.good() )
//     std::cout << "not good" << std::endl;
// outFile << str;
// outFile.close();


// int Request::parseEncodingBody( void ) {
//     std::cout << "encoding" << std::endl;
//     return 1;
// }

// int Request::parseEncodingBody( void ) {

//     std::string buffer;
//     std::string chunked;
//     std::string body = this->getBody();
//     size_t lengthToRead;

//     while ( body.size() ) {
        
//         buffer = body.substr( 0, body.find( "\r\n" ) );
//         if ( buffer.size() ) {
            
//             lengthToRead = hexadecimalToDecimal( buffer );
//             if ( !lengthToRead ) {

//                 break;
//             }
//             body.erase( 0, buffer.size() + std::strlen( "\r\n" ) );
//             chunked = body.substr( 0, lengthToRead );
            
//             if ( chunked.find( this->getStartBoundary() ) != std::string::npos &&
//                 (   chunked.find( "name=" ) != std::string::npos ||
//                     chunked.find( "filename=" ) != std::string::npos ||
//                     chunked.find( "Content-Type: " ) != std::string::npos ) ) { // body header

//                 // std::cout << GREEN << chunked << RESET << std::endl;
                
//                 this->setBHFilename( "" );
//                 this->setBHName( "" );
//                 this->setBHContentDispo( "" );
//                 this->setBHContentType( "" );
//                 this->parsePostBodyHeader( chunked );
                
//             } else if ( chunked.find( this->getEndBoundary() ) != std::string::npos ) { // end boundary

//             } else { // body

//                 // std::cout << RED << chunked << RESET << std::endl;
//                 // std::cout << "buffer: " << buffer << std::endl;
//                 // std::cout << "length: " << lengthToRead << std::endl;
//                 std::cout << "size: " << chunked.size() << std::endl;
//                 // exit( 1 );
//                 if ( !this->createFileAndWrite( chunked ) )
//                     return 0;
//                 if ( this->getBHFilename().empty() ) {
                    
//                     if ( !this->getBHName().empty() ) {

//                         if ( this->getPath().find( "cgi-bin/" ) != std::string::npos ) { // cgi post

//                         }
//                     }
//                 }
//             }
//             body.erase( 0, lengthToRead );
//         } else {

//             body.erase( 0, buffer.size() + std::strlen( "\r\n" ) );
//             continue;
//         }
//     }
//     return 1;
// }

// int Request::parseBoundariesBody( void ) {

//     std::string buffer;
//     std::string chunked;
//     std::string body = this->getBody();
//     size_t find;
//     size_t next;
    
//     while ( !body.empty() ) {
        
//         find = body.find( "\r\n" );
//         if ( find != std::string::npos ) {
            
//             buffer = body.substr( 0, find );
            
//             if ( buffer == this->getEndBoundary() )
//                 return 1;
//             else if ( buffer == this->getStartBoundary() ) {

//                 body.erase( 0, find + std::strlen( "\r\n" ) );
//                 find = body.find( "\r\n\r\n" );
//                 if ( find != std::string::npos ) {

//                     chunked = body.substr( 0, find );
//                     this->parsePostBodyHeader( chunked );
//                     std::cout << RED << chunked << RESET << std::endl;
//                 }

//                 body.erase( 0, find + std::strlen( "\r\n\r\n" ) );
            
//                 find = body.find( this->getStartBoundary() );
//                 if ( find != std::string::npos ) {
                        
//                     find = body.find( this->getStartBoundary() );
//                     if ( find != std::string::npos ) {

//                         next = body.find( "\r\n", find );
//                         chunked = body.substr( find, next - find );
//                         if ( chunked == this->getEndBoundary() ) {

//                             std::cout << chunked.size() << std::endl;
//                             chunked = body.substr( 0, find - 2 );
//                             if ( !this->createFileAndWrite( chunked ) )
//                                 return 0;
//                             body.erase( 0, find );
//                             return 1;
//                         } else if ( chunked == this->getStartBoundary() ) {
                            
//                             chunked = body.substr( 0, find - 2 );
//                             if ( !this->createFileAndWrite( chunked ) )
//                                 return 0;
//                             body.erase( 0, find );
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     return 1;
// }

int Request::parsePostBody( std::string const &buffer ) {
    
    int status = this->bufferPostBody( buffer );
    
    if ( !status ) {

        if ( this->getBodyType() == ENCODING )
            return 0;
        else if ( this->getBodyType() == BOUNDARIES )
            return 0;
        else if ( this->getBodyType() == LENGTH )
            return 0;
    } else if ( status == 2 )
        return 2;
    return 1;
}