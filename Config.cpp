/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:54:44 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 18:54:53 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

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
