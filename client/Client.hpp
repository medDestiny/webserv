#pragma once

#include "../response/Response.hpp"
#include "../request/Request.hpp"

class Client {

    private:
        int sockfd;
        Request request;
        Response response;


    public:
        Client( void );
        ~Client( void );
        int getsockfd( void ) const;
        void setsockfd( int const &sockfd );
        // void initrequest( void );
};