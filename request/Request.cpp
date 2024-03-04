#include "Request.hpp"

Request::Request( void ) {

    this->sendedcontent = 0;
}

Request::~Request( void ) { }

int Request::getsendedcontent( void ) const {

    return this->sendedcontent;
}

void Request::setsendedcontent( int const &sendedcontent ) {

    this->sendedcontent = sendedcontent;
}