/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:12:49 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 22:50:38 by mmisskin         ###   ########.fr       */
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

void	ServerName::addHost(std::string host) { _hosts.insert(host); }

void	ServerName::addHosts(std::set<std::string> hosts)
{
	_hosts.insert(hosts.begin(), hosts.end());
}

std::set<std::string> const &	ServerName::getHosts(void) const { return (_hosts); }

/* ErrorPage member functions */
ErrorPage::ErrorPage(void) {}

ErrorPage::ErrorPage(ErrorPage const & src) { *this = src; }

ErrorPage	&ErrorPage::operator=(ErrorPage const & right)
{
	_errors = right._errors;
	return (*this);
}

ErrorPage::~ErrorPage(void) {}

void	ErrorPage::addErrorPage(std::pair<std::string, std::string> const & error)
{
	_errors.insert(error);
}

std::map<std::string, std::string> const &	ErrorPage::getErrorPages() const { return (_errors); }

/* ClientMaxBodySize member functions */
ClientMaxBodySize::ClientMaxBodySize(void) { _size = 1024; }

ClientMaxBodySize::ClientMaxBodySize(ClientMaxBodySize const & src) { *this = src; }

ClientMaxBodySize	&ClientMaxBodySize::operator=(ClientMaxBodySize const & right)
{
	_size = right.getSize();
	return (*this);
}

ClientMaxBodySize::~ClientMaxBodySize(void) {}

void	ClientMaxBodySize::setSize(size_t size) { _size = size; }

size_t	ClientMaxBodySize::getSize(void) const { return (_size); }
