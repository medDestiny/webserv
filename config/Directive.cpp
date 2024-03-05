/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:12:49 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/05 21:36:32 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

/* Directive base class */
Directive::Directive(void) : _is_set(false) {}
bool	Directive::empty(void) const { return (!_is_set); }

/* Listen member functions */
Listen::Listen() : _host(DEFAULT_HOST), _port(DEFAULT_PORT) {}

Listen::Listen(std::string host, std::string port) : _host(host), _port(port) {}

Listen::Listen(Listen const & src) { *this = src; }

Listen	&Listen::operator=(Listen const & right)
{
	_host = right.getHost();
	_port = right.getPort();
	_is_set = !right.empty();
	return (*this);
}

Listen::~Listen(void) {}

std::string	Listen::getHost(void) const { return (_host); }

std::string	Listen::getPort(void) const { return (_port); }

void	Listen::setHost(std::string host) { _host = host; _is_set = true; }

void	Listen::setPort(std::string port) { _port = port; _is_set = true; }


/* ServerName member functions */
ServerName::ServerName(void) {}

ServerName::ServerName(ServerName const & src) { *this = src; }

ServerName	&ServerName::operator=(ServerName const & right)
{
	_hosts = right._hosts;
	_is_set = !right.empty();
	return (*this);
}

ServerName::~ServerName(void) {}

void	ServerName::addHost(std::string host)
{
	if (std::find(_hosts.begin(), _hosts.end(), host) != _hosts.end())
		return ;
	_hosts.push_back(host);
	_is_set = true;
}

void	ServerName::addHosts(std::vector<std::string> hosts)
{
	for (std::vector<std::string>::iterator it = hosts.begin(); it != hosts.end(); it++)
	{
		if (std::find(_hosts.begin(), _hosts.end(), *it) == _hosts.end())
		{
			_hosts.push_back(*it);
			_is_set = true;
		}
	}
}

std::vector<std::string> const &	ServerName::getHosts(void) const { return (_hosts); }

/* ErrorPage member functions */
ErrorPage::ErrorPage(void) {}

ErrorPage::ErrorPage(ErrorPage const & src) { *this = src; }

ErrorPage	&ErrorPage::operator=(ErrorPage const & right)
{
	_errors = right._errors;
	_is_set = !right.empty();
	return (*this);
}

ErrorPage::~ErrorPage(void) {}

void	ErrorPage::addErrorPage(std::pair<std::string, std::string> const & error)
{
	_errors.insert(error);
	 _is_set = true;
}

std::map<std::string, std::string> const &	ErrorPage::getErrorPages() const { return (_errors); }

/* ClientMaxBodySize member functions */
ClientMaxBodySize::ClientMaxBodySize(void) { _size = DEFAULT_BODY_SIZE; }

ClientMaxBodySize::ClientMaxBodySize(ClientMaxBodySize const & src) { *this = src; }

ClientMaxBodySize	&ClientMaxBodySize::operator=(ClientMaxBodySize const & right)
{
	_size = right.getSize();
	_is_set = !right.empty();
	return (*this);
}

ClientMaxBodySize::~ClientMaxBodySize(void) {}

void	ClientMaxBodySize::setSize(size_t size) { _size = size; _is_set = true; }

size_t	ClientMaxBodySize::getSize(void) const { return (_size); }

/* Return member functions */
Return::Return(void) { _code = 0; }

Return::Return(Return const & src) { *this = src; }

Return	&Return::operator=(Return const & right)
{
	_code = right.getCode();
	_url = right.getUrl();
	_is_set = !right.empty();
	return (*this);
}

Return::~Return(void) {}

int		Return::getCode(void) const { return (_code); }

void	Return::setCode(int code) { _code = code; _is_set = true; }

std::string	const &	Return::getUrl(void) const { return (_url); }

void				Return::setUrl(std::string const & url) { _url = url; }

/* Return member functions */
Root::Root(void) {}

Root::Root(Root const & src) { *this = src; }

Root	&Root::operator=(Root const & right)
{
	_path = right.getPath();
	_is_set = !right.empty();
	return (*this);
}

Root::~Root(void) {}

std::string const &	Root::getPath(void) const { return (_path); }

void				Root::setPath(std::string const & path) { _path = path; _is_set = true; }

/* AutoIndex member functions */
AutoIndex::AutoIndex(void) { _toggle = false; }

AutoIndex::AutoIndex(AutoIndex const & src) { *this = src; }

AutoIndex	&AutoIndex::operator=(AutoIndex const & right)
{
	_toggle = right.getToggle();
	_is_set = !right.empty();
	return (*this);
}

AutoIndex::~AutoIndex(void) {}

void	AutoIndex::setToggle(bool toggle) { _toggle = toggle; _is_set = true; }

bool	AutoIndex::getToggle(void) const { return (_toggle); }

/* Index member functions */
Index::Index(void) {}

Index::Index(Index const & src) { *this = src; }

Index	&Index::operator=(Index const & right)
{
	_index = right._index;
	_is_set = !right.empty();
	return (*this);	
}

Index::~Index(void) {}

void	Index::addIndex(std::string index)
{
	if (std::find(_index.begin(), _index.end(), index) != _index.end())
		return ;
	_index.push_back(index);
	_is_set = true;
}

void	Index::addIndexes(std::vector<std::string> indexes)
{
	for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
	{
		if (std::find(_index.begin(), _index.end(), *it) == _index.end())
		{
			_index.push_back(*it);
			_is_set = true;
		}
	}
}

std::vector<std::string> const &	Index::getIndexes(void) const { return (_index); }

/* UploadStore member functions */
UploadStore::UploadStore(void) { _path = DEFAULT_UPLOAD_PATH; }

UploadStore::UploadStore(UploadStore const & src) { *this = src; }

UploadStore	&UploadStore::operator=(UploadStore const & right)
{
	_path = right.getPath();
	_is_set = !right.empty();
	return (*this);
}

UploadStore::~UploadStore(void) {}

std::string const &	UploadStore::getPath(void) const { return (_path); }

void				UploadStore::setPath(std::string const & path) { _path = path; _is_set = true; }

/* LimitExcept member functions */
LimitExcept::LimitExcept(void) {}

LimitExcept::LimitExcept(LimitExcept const & src) { *this = src; }

LimitExcept	&LimitExcept::operator=(LimitExcept const & right)
{
	_methods = right._methods;
	_is_set = !right.empty();
	return (*this);
}

LimitExcept::~LimitExcept(void) {}

void							LimitExcept::setMethods(std::set<std::string> const & methods) { _methods = methods; }

std::set<std::string> const &	LimitExcept::getMethods(void) const { return (_methods); }
