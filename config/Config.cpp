/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:54:44 by mmisskin          #+#    #+#             */
/*   Updated: 2024/04/05 01:46:03 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Location.hpp"
#include "../server/Colors.hpp"
#include <iostream>

using namespace	Conf;

Config::Config(void) : _valid(false) {}

Config::Config(Config const & src) { *this = src; }

Config	&Config::operator=(Config const & right)
{
	_valid = right.isValid();
	_servers = right.getServers();

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
	ServerName					server_name = server.getServerName();
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (server.getListen() == it->getListen())
		{
			std::vector<std::string>	hosts = it->getServerName().getHosts();
			std::vector<std::string>	server_names = server_name.getHosts();
			std::vector<std::string>::iterator	name;
			for (std::vector<std::string>::iterator i = hosts.begin(); i != hosts.end(); i++)
			{
				name = std::find(server_names.begin(), server_names.end(), *i);
				if (name != server_names.end())
				{
					std::cerr << ORANGE
							  << "[warn] conflicting server name \""
							  << *i << "\""
							  << " on "
							  << it->getListen().getHost() << ":"
							  << it->getListen().getPort() 
							  << ", ignored" 
							  << RESET
							  << std::endl;
					server_names.erase(name);
				}
			}
			if (server_names.size() == 0)
				return ;
			server_name.setHosts(server_names);
			server.setServerName(server_name);
		}
	}
	_servers.push_back(server);
}

std::vector<Server> const &	Config::getServers(void) const
{
	return (_servers);
}

Server	Config::getServer(Conf::Server const & default_server, std::string server_name)
{
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->getListen() == default_server.getListen())
		{
			std::vector<std::string>	hosts = it->getServerName().getHosts();
			if (std::find(hosts.begin(), hosts.end(), server_name) != hosts.end())
				return (*it);
		}
	}
	return (default_server);
}
