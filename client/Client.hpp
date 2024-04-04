/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:46 by del-yaag          #+#    #+#             */
/*   Updated: 2024/04/04 18:40:29 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../response/Response.hpp"
#include "../request/Request.hpp"
#include "../config/Server.hpp"
#include "../config/Config.hpp"
#include "../client/Tools.hpp"

#include <ctime>

class Server;

class Client {

    private:
        int sockfd;
        std::time_t timeout;
        Conf::Server server;
        Conf::Server defaultServer;
        Config config;
        Request request;
        Response response;
        bool endRecHeader;


    public:
        Client( void );
        ~Client( void );
        Client(Client const & src);
        Client	&operator=(Client const & obj);
        int getsockfd( void ) const;
        void setsockfd( int const &sockfd );
        std::time_t gettimeout( void ) const;
        void settimeout( std::time_t const &timeout );
        Conf::Server getserver( void ) const;
        void setserver( Conf::Server const &server );
        Config getConfig( void );
        void setConfig( Config config );
        bool getEndRecHeader( void );
        void setEndRecHeader( bool endRecHeader );
        Response &getResponse( void );
        Request getRequest( void ) const;
        Conf::Server const &getDefaultServer( void ) const;
        void setDefaultServer( Conf::Server const &server );
        int sendPostResponse( std::string const &message );
        int recieveRequest();
        int sendresponse();
        int sendFileChunks( void );
};
