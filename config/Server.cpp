/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:30:01 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/12 16:51:59 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"

using namespace	Conf;

void	Server::setListen(Listen const & listen) { _listen = listen; }

Listen const	&Server::getListen(void) const { return (_listen); }

void				Server::setServerName(ServerName const & server_name) { _server_name = server_name; }

void				Server::addServerName(std::vector<std::string> server_names) { _server_name.addHosts(server_names); }

ServerName const	&Server::getServerName(void) const { return (_server_name); }

void				Server::setErrorPage(ErrorPage const & error_page) { _error_page = error_page; }

void				Server::addErrorPage(std::pair<std::string, std::string> const & error_page) { _error_page.addErrorPage(error_page); }

ErrorPage const &	Server::getErrorPage(void) const { return (_error_page); }

void						Server::setClientMaxBodySize(ClientMaxBodySize const & size) { _client_max_body_size = size; }

ClientMaxBodySize const &	Server::getClientMaxBodySize(void) const { return (_client_max_body_size); }


void				Server::setReturn(Return const & _return) { this->_return = _return; }

Return const &		Server::getReturn(void) const { return (_return); }

void				Server::setRoot(Root const & root) { _root = root; }

Root const &		Server::getRoot(void) const { return (_root); }

void				Server::setAutoIndex(AutoIndex const & autoindex) { _autoindex = autoindex; }

AutoIndex const &	Server::getAutoIndex(void) const { return (_autoindex); }

void				Server::setIndex(Index const & index) { _index = index; }

void				Server::addIndex(std::vector<std::string> const & index) { _index.addIndexes(index); }

Index const &		Server::getIndex(void) const { return (_index); }

void				Server::setUploadPath(UploadStore const & upload) { _upload_store = upload; }

UploadStore const &	Server::getUploadPath(void) const { return (_upload_store); }

void										Server::addLocation(std::pair<std::string, Location> location) { _locations.insert(location); }

std::map<std::string, Location> const &		Server::getLocations(void) const { return (_locations); }

void										Server::setLocations(std::map<std::string, Location> const & locations) {_locations = locations; }

std::map<std::string, Location>::iterator	Server::getLocation(std::string const & path) { return (_locations.find(path)); }
