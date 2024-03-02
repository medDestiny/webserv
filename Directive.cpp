/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:12:49 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/02 11:38:23 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

/* Listen member functions */
Listen::Listen() : _host(DEFAULT_HOST), _port(DEFAULT_PORT) {}

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
ClientMaxBodySize::ClientMaxBodySize(void) { _size = DEFAULT_BODY_SIZE; }

ClientMaxBodySize::ClientMaxBodySize(ClientMaxBodySize const & src) { *this = src; }

ClientMaxBodySize	&ClientMaxBodySize::operator=(ClientMaxBodySize const & right)
{
	_size = right.getSize();
	return (*this);
}

ClientMaxBodySize::~ClientMaxBodySize(void) {}

void	ClientMaxBodySize::setSize(size_t size) { _size = size; }

size_t	ClientMaxBodySize::getSize(void) const { return (_size); }

/* Return member functions */
Return::Return(void) { _code = 0; }

Return::Return(Return const & src) { *this = src; }

Return	&Return::operator=(Return const & right)
{
	_code = right.getCode();
	return (*this);
}

Return::~Return(void) {}

int		Return::getCode(void) const { return (_code); }

void	Return::setCode(int code) { _code = code; }

/* Return member functions */
Root::Root(void) {}

Root::Root(Root const & src) { *this = src; }

Root	&Root::operator=(Root const & right)
{
	_path = right.getPath();
	return (*this);
}

Root::~Root(void) {}

std::string const &	Root::getPath(void) const { return (_path); }

void				Root::setPath(std::string const & path) { _path = path; }

/* AutoIndex member functions */
AutoIndex::AutoIndex(void) { _toggle = false; }

AutoIndex::AutoIndex(AutoIndex const & src) { *this = src; }

AutoIndex	&AutoIndex::operator=(AutoIndex const & right)
{
	_toggle = right.getToggle();
	return (*this);
}

AutoIndex::~AutoIndex(void) {}

void	AutoIndex::setToggle(bool toggle) { _toggle = toggle; }

bool	AutoIndex::getToggle(void) const { return (_toggle); }

/* Index member functions */
Index::Index(void) {}

Index::Index(Index const & src) { *this = src; }

Index	&Index::operator=(Index const & right)
{
	_index = right._index;
	return (*this);	
}

Index::~Index(void) {}

void							Index::addIndex(std::string index) { _index.insert(index); }

void							Index::addIndexes(std::set<std::string> indexes)
{
	_index.insert(indexes.begin(), indexes.end());
}

std::set<std::string> const &	Index::getIndexes(void) const { return (_index); }

/* UploadStore member functions */
UploadStore::UploadStore(void) { _path = DEFAULT_UPLOAD_PATH; }

UploadStore::UploadStore(UploadStore const & src) { *this = src; }

UploadStore	&UploadStore::operator=(UploadStore const & right)
{
	_path = right.getPath();
	return (*this);
}

UploadStore::~UploadStore(void) {}

std::string const &	UploadStore::getPath(void) const { return (_path); }

void				UploadStore::setPath(std::string const & path) { _path = path; }
