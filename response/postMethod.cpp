#include "Response.hpp"
#include "../request/Request.hpp"

std::string Response::getBHName( void ) const {
    
    return this->BHName;
}

void Response::setBHName( std::string const &name ) {
    
    this->BHName = name;
}

std::string Response::getBHFilename( void ) const {
    
    return this->BHFilename;
}

void Response::setBHFilename( std::string const &filename ) {
    
    this->BHFilename = filename;
}

std::string Response::getBHContentDispo( void ) const {
    
    return this->BHContentDispo;
}

void Response::setBHContentDispo( std::string const &content ) {
    
    this->BHContentDispo = content;
}

std::string Response::getBHContentType( void ) const {
    
    return this->BHContentType;
}

void Response::setBHContentType( std::string const &type ) {
    
    this->BHContentType = type;
}

std::string Response::getBody( void ) const {

    return this->body;
}

void Response::setBody( std::string const &body ) {

    this->body = body;
}

std::string  Response::getHeaderValue( std::string const &chunck, std::string const &findStr, std::string const &delim ) {

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

void  Response::parsePostBodyHeader( std::string const &chunck ) {
    
    this->BHContentDispo = this->getHeaderValue( chunck, "Content-Disposition: ", ";" ); // get content desposition
    // std::cout << this->BHContentDispo << std::endl;

    this->BHName = this->getHeaderValue( chunck, "name=\"", "\"" ); // get name
    // std::cout << this->BHName << std::endl;

    this->BHFilename = this->getHeaderValue( chunck, "filename=\"", "\"" ); // get filename
    // std::cout << this->BHFilename << std::endl;
    
    this->BHContentType = this->getHeaderValue( chunck, "Content-Type: ", "\r\n" ); // get content type
    // std::cout << this->getBHContentType() << std::endl;
}

int  Response::createFileAndWrite( std::string const &str, bool const flag ) {

    std::string path;
    ssize_t bytes;

    if ( flag == true ) {
        
        if ( !this->getBHFilename().empty() ) {

            // std::cout << str << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
            bytes = write( this->fd, str.c_str(), str.size() );
            if ( bytes == -1 ) {

                std::cout << RED << "==> ERROR cannot write, in post method" << RESET << std::endl;
                return 0;
            }
        }
        
    } else {
        
        if ( !this->getBHFilename().empty() ) {
                        
            // std::cout << str << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
            path = "/Users/del-yaag/goinfre/file/" + this->getBHFilename();
            this->fd = open( path.c_str(), O_WRONLY | O_CREAT, 0777 );
            
            if ( fd == -1 ) {
                std::cout << "rah mabghach am3elem" << std::endl;
                return 0;
            }
            std::cout << "size of str: " << str.size() << std::endl;
            bytes = write( this->fd, str.c_str(), str.size() );
            if ( bytes == -1 ) {
                
                std::cout << "khouna rah mabghach ykteb" << std::endl;
                return 0;
            }
        }
    }
    return 1;
}

void Response::resetHeaderElements( void ) {

    this->setBHContentDispo( "" );
    this->setBHContentType( "" );
    this->setBHFilename( "" );
    this->setBHName( "" );
}

int Response::PutChunkedBodyToFile( Request const &request, bool const flag ) {

    std::string chunked;
    std::string chunkedBody;
    size_t find;
    size_t next;

    chunkedBody = this->body.substr( 0, CHUNKED );
    find = chunkedBody.find( request.getStartBoundary() );
    if ( find != std::string::npos ) {

        next = chunkedBody.find( "\r\n", find );
        chunked = chunkedBody.substr( find, next - find );
        if ( chunked == request.getEndBoundary() ) { // got the end boundaries

            std::cout << "false end" << std::endl;
            chunked = chunkedBody.substr( 0, find - 2 );
            if ( !this->createFileAndWrite( chunked, flag ) )
                return 0; // error
                
            this->body.erase( 0, find ); // erase body
            std::cout << "body: " << this->body.size() << std::endl;
            
            // reset all vars
            this->resetHeaderElements();
            if ( flag )
                this->bodyFlag = false;
            close( this->fd );
            
            return 0; // end

        } else if ( chunked == request.getStartBoundary() ) { // got the start boundaries
            
            std::cout << "false start" << std::endl;
            chunked = chunkedBody.substr( 0, find - 2 );
            if ( !this->createFileAndWrite( chunked, flag ) )
                return 0;
            this->body.erase( 0, find ); // erase body
            std::cout << "body: " << this->body.size() << std::endl;
            // reset all vars
            this->resetHeaderElements();
            if ( flag )
                this->bodyFlag = false;
            close( this->fd );
        }
    } else {

        std::cout << "false walo" << std::endl;
        if ( !this->createFileAndWrite( chunkedBody, flag ) )
            return 0;
        this->body.erase( 0, CHUNKED ); // erase body
        std::cout << "body: " << this->body.size() << std::endl;
        if ( !flag )
            this->bodyFlag = true;
    }
    return 1;
}

int  Response::parseBoundariesBody( Request const &request ) {

    std::string buffer;
    std::string chunked;
    std::string chunkedBody;
    size_t find;

    if ( this->bodyFlag == false ) {

        find = this->body.find( "\r\n" );
        if ( find != std::string::npos ) {
            
            buffer = this->body.substr( 0, find );
            
            if ( buffer == request.getEndBoundary() )
                return 0;
            else if ( buffer == request.getStartBoundary() ) {

                this->body.erase( 0, find + std::strlen( "\r\n" ) );
                find = this->body.find( "\r\n\r\n" );
                if ( find != std::string::npos ) { // get header.

                    chunked = this->body.substr( 0, find );
                    this->parsePostBodyHeader( chunked );
                    std::cout << RED << chunked << RESET << std::endl;
                }
                this->body.erase( 0, find + std::strlen( "\r\n\r\n" ) );

                if ( !this->PutChunkedBodyToFile( request, false ) )
                    return 0;
            }
        }
    } else {
        
        if ( !this->PutChunkedBodyToFile( request, true ) )
            return 0;
    }
    return 1;
}

int  Response::parseLengthBody( void ) {

    std::cout << "length" <<  std::endl;
    return 1;
}

int Response::parseEncodingBody( void ) {
    std::cout << "encoding" << std::endl;
    return 1;
}

int Response::execPostMethod( Request const &request ) {

    if ( request.getBodyType() == ENCODING ) {

        this->parseEncodingBody();
    } else if ( request.getBodyType() == BOUNDARIES ) {
        
        if ( !this->parseBoundariesBody( request ) ) {
            return 0;
        }
    }
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