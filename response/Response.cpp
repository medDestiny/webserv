/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoukhle <amoukhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:54:19 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/29 01:38:07 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../request/Request.hpp"
#include "../client/Client.hpp"

Response::Response( void ) {

    this->receivedcontent = 0;
    this->statusCode = 200;
    this->sendedHeader = false;
    this->displayError = false;
    this->autoIndexing = false;
    this->file = -2;
    this->countBytesRead = 0;
    this->contentResponse = 0;

    // ---- post ---- //
    this->bodyFlag = false;
}

Response::~Response( void ) { }

int Response::getreceivedcontent( void ) const {

    return this->receivedcontent;
}

void Response::setreceivedcontent( int const &receivedcontent ) {

    this->receivedcontent = receivedcontent;
}

void Response::setStatusCode( int const & statusCode ) {

    this->statusCode = statusCode;
}
int Response::getStatusCode( void ) const {

    return (this->statusCode);
}

void Response::setSendedHeader( bool const & sendedHeader ) {

    this->sendedHeader = sendedHeader;
}
bool Response::getSendedHeader( void ) const {

    return (this->sendedHeader);
}

void Response::setDisplayError( bool const & displayError ) {

    this->displayError = displayError;
}
bool Response::getDisplayError( void ) const {

    return (this->displayError);
}

void Response::setContentLength( size_t const & contentLength ) {

    this->contentLength = contentLength;
}
size_t Response::getContentLength( void ) const {

    return (this->contentLength);
}

void Response::setFile( int const & file ) {

    this->file = file;
}
int Response::getFile( void ) const {

    return (this->file);
}

void Response::setCountBytesRead( size_t const & countBytesRead ) {

    this->countBytesRead = countBytesRead;
}
size_t Response::getCountBytesRead( void ) const {

    return (this->countBytesRead);
}

void Response::setContentResponse( size_t const & contentResponse ) {

    this->contentResponse = contentResponse;
}
size_t Response::getContentResponse( void ) const {

    return (this->contentResponse);
}

void Response::setAutoIndexing( bool const & autoIndexing ) {

    this->autoIndexing = autoIndexing;
}
bool Response::getAutoIndexing( void ) const {

    return (this->autoIndexing);
}

std::string Response::getType( void ) const {

    return (this->type);
}
void Response::setType( std::string const & type ) {

    this->type = type;
}
std::string Response::getMimeType( void ) const {

    return (this->mimeType);
}
void Response::setMimeType( std::string const & mimetype ){

    this->mimeType = mimetype;
}

std::string Response::getStatusMessage(int const & statusCode) {
    std::map<int, std::string> statusMessages;
    statusMessages[200] = "OK";
    statusMessages[204] = "No Content";
    statusMessages[206] = "Partial Content";
    statusMessages[400] = "Bad Request";
    statusMessages[403] = "Forbidden";
    statusMessages[404] = "Not Found";
    statusMessages[405] = "Method Not Allowed";
    statusMessages[413] = "Payload Too Large";
    statusMessages[415] = "Unsupported Media Type";
    statusMessages[500] = "Internal Server Error";
    statusMessages[501] = "Not Implemented"; // The request method is not supported
    statusMessages[504] = "Gateway Timeout";
    statusMessages[505] = "HTTP Version Not Supported";

    std::map<int, std::string>::iterator it = statusMessages.find(statusCode);
    if (it != statusMessages.end()) {
        return it->second;
    } else {
        return "Unknown Status Code";
    }
}

ssize_t	Response::sendCgiHeader( int const sockfd, Request & request ) {

	std::ifstream	cgiFile(request.getCgi().getCgiOutFile());

	if (!cgiFile.good())
	{
		std::cerr << "unable to open cgi tmp file: " << strerror(errno) << std::endl;
		return (-1);
	}

	std::string	cgiHeader;
	std::string	tmp;

	std::string	statusLine = request.getHttpVersion() + " 200 OK";
	while (1)
	{
		if (!std::getline(cgiFile, tmp) || tmp.empty() || tmp == "\r")
			break;
		if (tmp.find("Status:") != std::string::npos)
		{
			std::string	status = tmp.substr(tmp.find(':') + 1);
			std::string message;

			if (status[0] == ' ' || status[0] == '\t')
				status.erase(0, 1);

			message = status.substr(4);
			status = status.substr(0, 3);
			statusLine = request.getHttpVersion() + ' ' + status + ' ' + message;

			/* prevent header sending on failure statusCode */
			size_t	statusCode = stringToInt(status);
			this->statusCode = statusCode;
			if (statusCode >= 400)
				return (0);

			continue ;
		}
		cgiHeader += "\r\n" + tmp;
	}
	cgiHeader = statusLine + cgiHeader;
	std::streampos	currentPos = cgiFile.tellg();
	if (cgiHeader.find("Content-Length:") == std::string::npos)
	{
		cgiFile.seekg(0, cgiFile.end);
		std::streampos	length = cgiFile.tellg() - currentPos;

		cgiHeader += "\r\nContent-Length: " + intToString(length);
		setContentLength(static_cast<size_t>(length));
	}
	cgiHeader += "\r\nConnection: " + request.getConnection();
	cgiHeader += "\r\n\r\n";
	cgiFile.close();
	std::cout << GREEN << cgiHeader << RESET << std::endl;

	/* open the file for later body reading */
	this->file = open( request.getCgi().getCgiOutFile().c_str(), O_RDONLY );
	if ( this->file == -1 ) {
		std::cerr << "failed to open file" << std::endl;
		return (-1); // to remove client and poll
	}
	/* skip the header part of the file */
	char	buff[static_cast<size_t>(currentPos)];
	read(this->file, buff, currentPos);

    ssize_t sended;
    sended = send( sockfd, cgiHeader.c_str(), cgiHeader.length(), 0 );
	return (sended);
}

ssize_t Response::sendHeader( int const &sockfd, Request const & request ) {

    std::string statusLine;

    // ----------status line----------- //
        if (!request.getRangeStart().empty()) {
            this->statusCode = 206;
        }
        std::string absolutPath = request.getLocation().getRoot().getPath() + request.getUrl();
        if (request.getCheckLocation() && absolutPath.back() != '/' && isDirectory(absolutPath.c_str())) {
            this->statusCode = 302;
        }
    statusLine = request.getHttpVersion() + " " + intToString(this->statusCode) + " " + getStatusMessage(this->statusCode);

    std::string headerResponse;
    headerResponse = statusLine + "\r\nContent-Type: " + this->mimeType + "\r\nContent-Length: ";
    if (!request.getRangeStart().empty()) {
        headerResponse += intToString( request.getRangeEndNum() - request.getRangeStartNum() + 1 ) + "\r\nAccept-Ranges: bytes";
        headerResponse += "\r\nContent-Range: bytes " + request.getRangeStart() + "-" + request.getRangeEnd() + "/" + intToString( this->contentLength );
        this->contentLength = request.getRangeEndNum() - request.getRangeStartNum() + 1;
    }
    else
    {
        headerResponse += intToString( this->contentLength );
        if (mimeType.substr(0, mimeType.find('/')) == "video" || mimeType.substr(0, mimeType.find('/')) == "audio")
            headerResponse += "\r\nAccept-Ranges: bytes";
    }
    headerResponse += "\r\nConnection: " + request.getConnection();
    if (request.getCheckLocation() && absolutPath.back() != '/' && isDirectory(absolutPath.c_str())) {
        headerResponse += "\r\nLocation: " + request.getStringLocation() + "/";
    }
    headerResponse += "\r\n\r\n";

    ssize_t sended;
    sended = send( sockfd, ( headerResponse.c_str() ), headerResponse.length(), 0 );
    return (sended);
}

ssize_t Response::sendBody( int const &sockfd, Request const & request ) {

    // -----------open file---------- //
    if (this->file == -2) {
        this->file = open( request.getPath().c_str(), O_RDONLY, 0777 );
        if ( this->file == -1 ) {
            std::cerr << "failed to open file" << std::endl;
            this->statusCode = 500;
            return (1); // error
        }
    }

    char buffer[SENDED];
    char bufferS[1000000];
    ssize_t sended = 0;
    // --------seek the file-------- //
    if ( !request.getRangeStart().empty() && this->countBytesRead < request.getRangeStartNum() ) {
        size_t bufferSize = 1000000;
        if (request.getRangeStartNum() - this->countBytesRead < 1000000)
            bufferSize = request.getRangeStartNum() - this->countBytesRead;
        size_t bytesRead = read(this->file, bufferS, bufferSize);
        this->countBytesRead += bytesRead;
    }
    else { // --------read file to send----------//
        size_t bytesRead = read(this->file, buffer, SENDED);
        if (bytesRead < SENDED)
            buffer[bytesRead] = '\0';
        this->contentResponse += bytesRead;
        std::string message = std::string(buffer, bytesRead);
        
        // std::string tmp = message;
        while ( !message.empty() ) {
            
            std::string str = message.substr( 0, 1024 );
            // std::cout << RED << "HELLO" << RESET << std::endl;
            sended = send( sockfd, str.c_str() , str.size(), 0 );
            message.erase( 0, 1024 );
        }
        return (sended);
    }
    return (1);
}

std::string Response::getErrorPage(std::map<std::string, std::string> ErrorPages) {

    std::map<std::string, std::string>::iterator it = ErrorPages.find(intToString(this->statusCode));
    if (it != ErrorPages.end())
        return (it->second);
    else
        return ("");
}

void Response::displayErrorPage( Conf::Server & server, int const &sockfd, Request request) {

    std::string header;
    std::string message;
    std::string body;

    std::string errorPage = getErrorPage(server.getErrorPage().getErrorPages());
    if (!errorPage.empty()) {
        errorPage = server.getRoot().getPath() + errorPage;
        if (access(errorPage.c_str(), F_OK) == -1) {
            errorPage = "";
        }
    }
    if (request.getCheckLocation()) {
        errorPage = getErrorPage(request.getLocation().getErrorPage().getErrorPages());
        if (!errorPage.empty()) {
            errorPage = request.getLocation().getRoot().getPath() + errorPage;
            if (access(errorPage.c_str(), F_OK) == -1) {
                errorPage = "";
            }
        }
    }

    if (errorPage.empty()) {
        body += "<!DOCTYPE html>\n";
        body += "<html>\n";
        body += "<head><title>" + intToString(this->statusCode) + " " + getStatusMessage(this->statusCode) + "</title></head>\n";
        body += "<body>\n";
        body += "<center><h1>" + intToString(this->statusCode) + " " + getStatusMessage(this->statusCode) + "</h1></center>\n";
        body += "</body>\n";
        body += "</html>";
    }
    else {
        std::ifstream streamFile;
        std::string tmp;
        streamFile.open(errorPage);
        while (std::getline( streamFile, tmp)) {
            body += tmp;
            body += '\n';
        }
        streamFile.close();
    }

    header += "HTTP/1.1 " + intToString(this->statusCode) + " " + getStatusMessage(this->statusCode) + "\r\n";
    header += "Content-Type: text/html\r\n";
    header += "Content-Length: ";
    if (errorPage.empty())
        header += intToString(body.length());
    else
        header += intToString( get_size_fd(errorPage) );
    header += "\r\nConnection: close\r\n\r\n";

    message = header + body;
    send( sockfd, message.c_str(), message.length(), 0);

}

int Response::displayAutoIndex( Conf::Server & server, int const &sockfd, Request request ) {

    std::vector<std::string> fileNames;
    DIR* dir;
    struct dirent* entry;
    std::string fullPath;

    if (request.getCheckLocation())
        fullPath = request.getLocation().getRoot().getPath() + request.getStringLocation() + "/";
    else
        fullPath = server.getRoot().getPath() + request.getUrl();
    dir = opendir(fullPath.c_str());
    if (dir == NULL) {
        std::cerr << "Error opening directory" << std::endl;
        this->statusCode = 500;
        return (0);
    }
    while ((entry = readdir(dir)) != NULL) {
        std::string str = std::string(entry->d_name);
            if (str != "." && str != "..")
                fileNames.push_back( str );
    }
    closedir(dir);

    std::string body;
    std::string header;
    std::string message;

    body += "<html>\n";
    body += "<head><title>Index of /</title></head>\n";
    body += "<body>\n";
    body += "<h1>Index of /</h1><hr><pre><a href=\"../\">../</a>\n";
    for (std::vector<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
        if (request.getCheckLocation())
            body += "<a href=" + request.getStringLocation() + "/" + *it + ">" + *it + "</a>\n";
        else {
            if (request.getUrl() == "/")
                body += "<a href=\"" + request.getUrl().erase(0, 1) + "/" + *it + "\">" + *it + "</a>\n";
            else
                body += "<a href=\"" + request.getUrl() + "/" + *it + "\">" + *it + "</a>\n";
        }
    }
    body += "</pre><hr></body>\n";
    body += "</html>";

    header = "HTTP/1.1 " + intToString(this->statusCode) + " " + getStatusMessage(this->statusCode) + "\r\n";
    header += "Content-Type: text/html\r\n";
    header += "Content-Length: " + intToString(body.length());
    header += "\r\nConnection: " + request.getConnection() + "\r\n\r\n";

    message = header + body;
    ssize_t sended = send( sockfd, message.c_str(), message.length(), 0);
    if (sended == -1) {
        this->statusCode = 500;
        return (0); // error
    }
    return (1);
}

int Response::deleteResource(int const sockfd, Request request) {
    if ( isDirectory( request.getPath().c_str()) ) {
        if ( rmdir( request.getPath().c_str() ) ) {
            this->statusCode = 500;
            return (0); //error
        }
    }
    else
        if ( remove( request.getPath().c_str() ) ) {
            this->statusCode = 500;
            return (0); //error
        }
    std::string header;

    header = "HTTP/1.1 " + intToString(204) + " " + getStatusMessage(204) + "\r\n";
    header += "Connection: " + request.getConnection() + "\r\n\r\n";

    ssize_t sended = send( sockfd, header.c_str(), header.length(), 0);
    if (sended == -1) {
        this->statusCode = 500;
        return (0); // error
    }
    return (1);
}
