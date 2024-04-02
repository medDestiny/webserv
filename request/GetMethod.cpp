/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:05:33 by amoukhle          #+#    #+#             */
/*   Updated: 2024/04/01 22:18:28 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../response/Response.hpp"
#include "../client/Client.hpp"

void Request::getRange( void ) {
    std::string range;
    std::istringstream headerStream(this->header);
    while (std::getline(headerStream, range)) {
        if (range.substr(0, 13) == "Range: bytes=") {
            int numS = range.find('-') - (range.find('=') + 1);
            this->rangeStart = range.substr(range.find('=') + 1, numS);
            this->rangeStartNum = stringToInt(this->rangeStart);
            if (range.find('-') + 1 < range.length() - 1 )
                this->rangeEnd = range.substr(range.find('-') + 1);
            if (this->rangeEnd.empty()) {
                this->rangeEndNum = get_size_fd(this->path) - 1;
                this->rangeEnd = intToString(this->rangeEndNum);
            }
            else
                this->rangeEndNum = stringToInt(this->rangeEnd);
            break;
        }
    }
}

int Request::checkFile( Conf::Server & server, Response & response ) {

    if (this->checkLocation)
        this->path = location.getRoot().getPath() + "/" + this->path;
    else
        this->path = server.getRoot().getPath() + "/" + this->path;
    if (access(this->path.c_str(), F_OK) == -1) {
        response.setStatusCode( 404 );
        return (0);
    }
    if (access(this->path.c_str(), R_OK) == -1) {
        response.setStatusCode( 403 );
        return (0);
    }
    response.setType( this->getPath().substr(this->getPath().rfind('.') + 1) );
    response.setMimeType( getMimeType(response.getType()) );
    return (1);
}

int Request::checkDirectory( Conf::Server & server, Response & response ) {

    if (this->checkLocation)
        this->path = getIndex(this->location.getIndex().getIndexes(), this->location.getRoot().getPath() + this->stringLocation);
    else
        this->path = getIndex(server.getIndex().getIndexes(), server.getRoot().getPath());
    if (this->path.empty()) {
        bool checkAutoIndex = server.getAutoIndex().getToggle();
        if (this->checkLocation)
            checkAutoIndex = this->location.getAutoIndex().getToggle();
        if (!checkAutoIndex) {
            response.setStatusCode( 403 );
            return (0);
        }
        else {
            response.setAutoIndexing( true );
            return (1);
        }
    }
    return (1);
}