#include "Client.hpp"

Client::Client( void ) { }

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