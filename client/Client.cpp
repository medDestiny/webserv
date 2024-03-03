#include "Client.hpp"

Client::Client( void ) { }

Client::~Client( void ) { }

int Client::getsockfd( void ) const {

    return this->sockfd;
}

void Client::setsockfd( int const &sockfd ) {

    this->sockfd = sockfd;
}

// void Client::initrequest( void ) {

//     this->request.
// }