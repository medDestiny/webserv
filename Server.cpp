/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:30:01 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 22:50:31 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::setListen(Listen const & listen) { _listen = listen; }

Listen const	&Server::getListen(void) const { return (_listen); }

void				Server::setServerName(ServerName const & server_name) { _server_name = server_name; }

void				Server::addServerName(std::set<std::string> server_names) { _server_name.addHosts(server_names); }

ServerName const	&Server::getServerName(void) const { return (_server_name); }

void				Server::addErrorPage(std::pair<std::string, std::string> const & error_page) { _error_page.addErrorPage(error_page); }

ErrorPage const &	Server::getErrorPage(void) const { return (_error_page); }

void						Server::setClientMaxBodySize(ClientMaxBodySize const & size) { _client_max_body_size = size; }

ClientMaxBodySize const &	Server::getClientMaxBodySize(ClientMaxBodySize const & size) { return (_client_max_body_size); }
