#pragma once

#include "../response/Response.hpp"
#include "../request/Request.hpp"

#include <ctime>

class Client {

    private:
        int sockfd;
        std::time_t timeout;
        Request request;
        Response response;


    public:
        Client( void );
        ~Client( void );
        int getsockfd( void ) const;
        void setsockfd( int const &sockfd );
        std::time_t gettimeout( void ) const;
        void settimeout( std::time_t const &timeout );
        // void initrequest( void );
};