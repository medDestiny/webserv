/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:46 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/05 15:54:47 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../response/Response.hpp"
#include "../request/Request.hpp"
#include "../config/Server.hpp"

#include <ctime>

class Client {

    private:
        int sockfd;
        std::time_t timeout;
        Conf::Server server;
        Request request;
        Response response;


    public:
        Client( void );
        ~Client( void );
        int getsockfd( void ) const;
        void setsockfd( int const &sockfd );
        std::time_t gettimeout( void ) const;
        void settimeout( std::time_t const &timeout );
        Conf::Server getserver( void ) const;
        void setserver( Conf::Server const &server );
        // void initrequest( void );
};