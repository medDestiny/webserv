/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:21:11 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 22:44:07 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

#include <string>
#include <map>
#include <set>

class	Listen
{
	public:
		Listen(void);
		Listen(Listen const & src);
		Listen	&operator=(Listen const & right);
		~Listen(void);
		
		Listen(std::string host, std::string port);

		std::string	getHost(void) const;
		std::string	getPort(void) const;
		void		setHost(std::string host);
		void		setPort(std::string port);

	private:
		std::string	_host;
		std::string	_port;
};

class	ServerName
{
	public:
		ServerName(void);
		ServerName(ServerName const & src);
		ServerName	&operator=(ServerName const & right);
		~ServerName(void);

		void							addHost(std::string host);
		void							addHosts(std::set<std::string> hosts);
		std::set<std::string> const &	getHosts(void) const;

	private:
		std::set<std::string>	_hosts;
};

class	ErrorPage
{
	public:
		ErrorPage(void);
		ErrorPage(ErrorPage const & src);
		ErrorPage	&operator=(ErrorPage const & right);
		~ErrorPage(void);


		std::map<std::string, std::string> const &	getErrorPages() const;
		void	addErrorPage(std::pair<std::string, std::string> const & error);

	private:
		std::map<std::string, std::string>	_errors;
};

class	ClientMaxBodySize
{
	public:
		ClientMaxBodySize(void);
		ClientMaxBodySize(ClientMaxBodySize const & src);
		ClientMaxBodySize	&operator=(ClientMaxBodySize const & right);
		~ClientMaxBodySize(void);

		void	setSize(size_t size);
		size_t	getSize(void) const;

	private:
		size_t	_size;
};

class	LimitExcept
{
	~LimitExcept(void);

	private:
		std::set<std::string>	_methods;
};

class	Return
{
	~Return(void);

	private:
		int	_code;
};

class	Root
{
	~Root(void);

	private:
		std::string	_path;
};

class	AutoIndex
{
	~AutoIndex(void);

	private:
		bool	_toggle;
};

class	Index
{
	~Index(void);

	private:
		std::set<std::string>	_index;
};

class	UploadStore
{
	~UploadStore(void);

	private:
		std::string	_path;
};

#endif
