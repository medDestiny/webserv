/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:54:44 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/02 12:11:17 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

using namespace	Conf;

Config::Config(void) : _valid(false) {}

Config::Config(Config const & src)
{
	*this = src;
}

Config	&Config::operator=(Config const & right)
{
	_valid = right.isValid();

	return (*this);
}

void	Config::setAsValid(void)
{
	this->_valid = true;
}

bool	Config::isValid(void) const
{
	return (_valid);
}

void	Config::addServer(Server server)
{
	_servers.push_back(server);
}

std::vector<Server> const &	Config::getServers(void) const
{
	return (_servers);
}

#include<iostream>
void	Config::print(void) const
{
	std::set<std::string>				hosts;
	std::set<std::string>				indexes;
	std::map<std::string, std::string>	errors;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "-------- Server N:" << i + 1 << " --------" << std::endl;
		std::cout << "Host: " << _servers[i].getListen().getHost() << std::endl;
		std::cout << "Port: " << _servers[i].getListen().getPort() << std::endl;
		std::cout << "Server names: ";
		hosts = _servers[i].getServerName().getHosts();
		for (std::set<std::string>::iterator it = hosts.begin(); it != hosts.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
		errors = _servers[i].getErrorPage().getErrorPages();
		std::cout << "Error Page: ";
		for (std::map<std::string, std::string>::iterator it = errors.begin(); it != errors.end(); it++)
		{
			std::cout << "\t\t" << "code:" << it->first << " " << "path:" << it->second << std::endl;
		}
		std::cout << "Client Max Body size (in bytes): " << _servers[i].getClientMaxBodySize().getSize() << std::endl;
		std::cout << "Http redirection: " << _servers[i].getReturn().getCode() << std::endl;
		std::cout << "Root: " << _servers[i].getRoot().getPath() << std::endl;
		std::cout << "Auto indexing: " << _servers[i].getAutoIndex().getToggle() << std::endl;
		std::cout << "Indexes: ";
		indexes = _servers[i].getIndex().getIndexes();
		for (std::set<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
		std::cout << "Upload Path: " << _servers[i].getUploadPath().getPath() << std::endl;
	}
}
