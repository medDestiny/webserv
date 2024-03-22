/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 15:19:45 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/22 22:56:36 by del-yaag         ###   ########.fr       */
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

std::string Request::getHeaderValue( std::string const &chunck, std::string const &findStr, std::string const &delim ) {

    size_t find;
    size_t next;
    std::string str;
    
    find = chunck.find( findStr );
    if ( find != std::string::npos ) {
        
        find += findStr.size();
        next = chunck.find( delim, find );
        if ( next != std::string::npos )
            str = chunck.substr( find, next - find );
    }
    
    return str;
}

void Request::parsePostBodyHeader( std::string const &chunck ) {
    
    this->BHContentDispo = this->getHeaderValue( chunck, "Content-Disposition: ", ";" ); // get content desposition
    // std::cout << this->BHContentDispo << std::endl;

    this->BHName = this->getHeaderValue( chunck, "name=\"", "\"" ); // get name
    // std::cout << this->BHName << std::endl;

    this->BHFilename = this->getHeaderValue( chunck, "filename=\"", "\"" ); // get filename
    // std::cout << this->BHFilename << std::endl;
    
    this->BHContentType = this->getHeaderValue( chunck, "Content-Type: ", "\r\n" ); // get content type
    // std::cout << this->getBHContentType() << std::endl;
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

int Request::createFileAndWrite( std::string const &str ) {

    // int fd;
    // std::string path;
    // ssize_t bytes;

    if ( !this->getBHFilename().empty() ) {
                    
        // path = "/Users/del-yaag/Desktop/" + this->getBHFilename();
        // fd = open( path.c_str(), O_WRONLY | O_CREAT, 0777 );
        // if ( fd == -1 ) {
        //     std::cout << "rah mabghach am3elem" << std::endl;
        //     return 0;
        // }
        // bytes = write( fd, str.c_str(), length );
        // if ( bytes == -1 ) {
            
        //     std::cout << "khouna rah mabghach ykteb" << std::endl;
        //     return 0;
        // }

        std::ofstream outFile( "/Users/del-yaag/Desktop/" + this->getBHFilename() );
        if ( !outFile.good() )
            std::cout << "not good" << std::endl;
        outFile << str;
        outFile.close();
    }
    return 1;
}

// int Request::parseEncodingBody( void ) {

//     std::string buffer;
//     // std::string chunked;
//     std::string body = this->getBody();
//     size_t lengthToRead;

//     std::istringstream stream( this->getBody() );
//     std::getline( stream, buffer, '\r' );
//     lengthToRead = hexadecimalToDecimal( buffer );
    
//     std::cout << "|" << buffer << "|" << std::endl;
//     return 1;
// }
int Request::parseEncodingBody( void ) {

    std::string buffer;
    std::string chunked;
    std::string body = this->getBody();
    size_t lengthToRead;

    while ( body.size() ) {
        
        buffer = body.substr( 0, body.find( "\r\n" ) );
        if ( buffer.size() ) {
            
            lengthToRead = hexadecimalToDecimal( buffer );
            if ( !lengthToRead ) {

                break;
            }
            body.erase( 0, buffer.size() + std::strlen( "\r\n" ) );
            chunked = body.substr( 0, lengthToRead );
            
            if ( chunked.find( this->getStartBoundary() ) != std::string::npos &&
                (   chunked.find( "name=" ) != std::string::npos ||
                    chunked.find( "filename=" ) != std::string::npos ||
                    chunked.find( "Content-Type: " ) != std::string::npos ) ) { // body header

                // std::cout << GREEN << chunked << RESET << std::endl;
                
                this->setBHFilename( "" );
                this->setBHName( "" );
                this->setBHContentDispo( "" );
                this->setBHContentType( "" );
                this->parsePostBodyHeader( chunked );
                
            } else if ( chunked.find( this->getEndBoundary() ) != std::string::npos ) { // end boundary

            } else { // body

                // std::cout << RED << chunked << RESET << std::endl;
                // std::cout << "buffer: " << buffer << std::endl;
                // std::cout << "length: " << lengthToRead << std::endl;
                std::cout << "size: " << chunked.size() << std::endl;
                // exit( 1 );
                if ( !this->createFileAndWrite( chunked ) )
                    return 0;
                if ( this->getBHFilename().empty() ) {
                    
                    if ( !this->getBHName().empty() ) {

                        if ( this->getPath().find( "cgi-bin/" ) != std::string::npos ) { // cgi post

                        }
                    }
                }
            }
            body.erase( 0, lengthToRead );
        } else {

            body.erase( 0, buffer.size() + std::strlen( "\r\n" ) );
            continue;
        }
    }
    return 1;
}

int Request::parseBoundariesBody( void ) {

    std::string buffer;
    std::string chunked;
    std::string body = this->getBody();
    size_t find;
    size_t next;
    
    while ( !body.empty() ) {
        
        find = body.find( "\r\n" );
        if ( find != std::string::npos ) {
            
            buffer = body.substr( 0, find );
            
            if ( buffer == this->getEndBoundary() )
                return 1;
            else if ( buffer == this->getStartBoundary() ) {

                body.erase( 0, find + std::strlen( "\r\n" ) );
                find = body.find( "\r\n\r\n" );
                if ( find != std::string::npos ) {

                    chunked = body.substr( 0, find );
                    this->parsePostBodyHeader( chunked );
                    std::cout << RED << chunked << RESET << std::endl;
                }

                body.erase( 0, find + std::strlen( "\r\n\r\n" ) );
            
                find = body.find( this->getStartBoundary() );
                if ( find != std::string::npos ) {
                        
                    find = body.find( this->getStartBoundary() );
                    if ( find != std::string::npos ) {

                        next = body.find( "\r\n", find );
                        chunked = body.substr( find, next - find );
                        if ( chunked == this->getEndBoundary() ) {

                            std::cout << chunked.size() << std::endl;
                            chunked = body.substr( 0, find - 2 );
                            if ( !this->createFileAndWrite( chunked ) )
                                return 0;
                            body.erase( 0, find );
                            return 1;
                        } else if ( chunked == this->getStartBoundary() ) {
                            
                            chunked = body.substr( 0, find - 2 );
                            if ( !this->createFileAndWrite( chunked ) )
                                return 0;
                            body.erase( 0, find );
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int Request::parseLengthBody( void ) {

    std::cout << "length" <<  std::endl;
    return 1;
}

int Request::parsePostBody( std::string const &buffer ) {
    
    int status = this->bufferPostBody( buffer );
    if ( !status ) {

        if ( this->getBodyType() == ENCODING )
            this->parseEncodingBody(); // implimanted *still*
        else if ( this->getBodyType() == BOUNDARIES )
            this->parseBoundariesBody(); // not implimanted yet
        else if ( this->getBodyType() == LENGTH )
            this->parseLengthBody(); // not implimanted yet
        return 0;
    } else if ( status == 2 )
        return 2;
    return 1;
}

// ---------------------------------- //
int hexadecimalToDecimal( std::string hexVal ) {

    int len = hexVal.size();
    int base = 1;
    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--) {

        if (hexVal[i] >= '0' && hexVal[i] <= '9') {

            dec_val += (int(hexVal[i]) - 48) * base;
            base = base * 16;
            
        } else if (hexVal[i] >= 'a' && hexVal[i] <= 'f') {

            dec_val += (int(hexVal[i]) - 87) * base;
            base = base * 16;
        } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {

            dec_val += (int(hexVal[i]) - 55) * base;
            base = base * 16;
        } 
    }
    return dec_val;
}