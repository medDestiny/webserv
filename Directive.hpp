/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:21:11 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 18:27:04 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

#include <string>
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

		void	addHost(std::string host);

	private:
		std::set<std::string>	_hosts;
};

class	ErrorPage
{
	~ErrorPage(void);

	private:
		std::string	_code; // might regret later and make this int
		std::string	_path;
};

class	ClientMaxBodySize
{
	~ClientMaxBodySize(void);

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
