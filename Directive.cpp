/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:12:49 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 18:47:39 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

/* Listen member functions */
Listen::Listen() : _host("0.0.0.0"), _port("80") {}

Listen::Listen(std::string host, std::string port) : _host(host), _port(port) {}

Listen::Listen(Listen const & src) { *this = src; }

Listen	&Listen::operator=(Listen const & right)
{
	_host = right.getHost();
	_port = right.getPort();

	return (*this);
}

Listen::~Listen(void) {}

std::string	Listen::getHost(void) const { return (_host); }

std::string	Listen::getPort(void) const { return (_port); }

void	Listen::setHost(std::string host) { _host = host; }

void	Listen::setPort(std::string port) { _port = port; }


/* ServerName member functions */
ServerName::ServerName(void) {}

ServerName::ServerName(ServerName const & src) { *this = src; }

ServerName	&ServerName::operator=(ServerName const & right)
{
	_hosts = right._hosts;
	return (*this);
}

ServerName::~ServerName(void) {}

void	ServerName::addHost(std::string host)
{
	_hosts.insert(host);
}


