/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:54:44 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/13 17:01:03 by mmisskin         ###   ########.fr       */
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

#include<iostream>
void	Config::print(void) const
{
	std::set<std::string>				methods;
	std::vector<std::string>				hosts;
	std::vector<std::string>				indexes;
	std::map<std::string, std::string>	errors;
	std::map<std::string, Location>		locations;
	std::map<std::string, std::string>	cgi;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "-------- Server N:" << i + 1 << " --------" << std::endl;
		std::cout << "Host: " << _servers[i].getListen().getHost() << std::endl;
		std::cout << "Port: " << _servers[i].getListen().getPort() << std::endl;
		std::cout << "Server names: ";
		hosts = _servers[i].getServerName().getHosts();
		for (std::vector<std::string>::iterator it = hosts.begin(); it != hosts.end(); it++)
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
		for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
		std::cout << "Upload Path: " << _servers[i].getUploadPath().getPath() << std::endl;
		
		// locations	
		locations = _servers[i].getLocations();
		for (std::map<std::string, Location>::iterator lo = locations.begin(); lo != locations.end(); lo++)
		{
			std::cout << "----->\t\tLocation: " << lo->first << " -------" << std::endl;
			errors = lo->second.getErrorPage().getErrorPages();
			std::cout << "\t\t" << "Error Page: ";
			for (std::map<std::string, std::string>::iterator it = errors.begin(); it != errors.end(); it++)
			{
				std::cout << "\t\t" << "code:" << it->first << " " << "path:" << it->second << std::endl;
			}
			std::cout << "\t\t" << "Client Max Body size (in bytes): " << lo->second.getClientMaxBodySize().getSize() << std::endl;
			std::cout << "\t\t" << "Http redirection: " << lo->second.getReturn().getCode() << std::endl;
			std::cout << "\t\t" << "Root: " << lo->second.getRoot().getPath() << std::endl;
			std::cout << "\t\t" << "Auto indexing: " << lo->second.getAutoIndex().getToggle() << std::endl;
			std::cout << "\t\t" << "Indexes: ";
			indexes = lo->second.getIndex().getIndexes();
			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
			{
				std::cout << "\t\t" << *it << " ";
			}
			std::cout << std::endl;
			std::cout << "\t\t" << "Upload Path: " << lo->second.getUploadPath().getPath() << std::endl;
			methods = lo->second.getLimitExcept().getMethods();
			std::cout << "\t\tAllowed Methods: ";
			for (std::set<std::string>::iterator m = methods.begin(); m != methods.end(); m++)
			{
				std::cout << "\t\t" << *m << " ";
			}
			std::cout << std::endl;
			cgi = lo->second.getCgiPass().getCgi();
			for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); it++)
			{
				std::cout << "\t\t" << "Cgi ext:" << it->first << " " << "cgi:" << it->second << std::endl;
			}
		}
	}
}
