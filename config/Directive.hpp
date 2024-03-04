/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:21:11 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/04 15:35:19 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

#include <string>
#include <map>
#include <set>

# define DEFAULT_HOST "0.0.0.0"
# define DEFAULT_PORT "80"
# define DEFAULT_BODY_SIZE 1048576 // 1M in bytes
# define DEFAULT_UPLOAD_PATH "/"

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
	public:
	~LimitExcept(void) {}

	private:
		std::set<std::string>	_methods;
};

class	Return
{
	public:
		Return(void);
		Return(Return const & src);
		Return	&operator=(Return const & right);
		~Return(void);

		int		getCode(void) const;
		void	setCode(int code);

	private:
		int	_code;
};

class	Root
{
	public:
		Root(void);
		Root(Root const & src);
		Root	&operator=(Root const & right);
		~Root(void);

		std::string const &	getPath(void) const;
		void				setPath(std::string const & path);

	private:
		std::string	_path;
};

class	AutoIndex
{
	public:
		AutoIndex(void);
		AutoIndex(AutoIndex const & src);
		AutoIndex	&operator=(AutoIndex const & right);
		~AutoIndex(void);

		void	setToggle(bool toggle);
		bool	getToggle(void) const;

	private:
		bool	_toggle;
};

class	Index
{
	public:
		Index(void);
		Index(Index const & src);
		Index	&operator=(Index const & right);
		~Index(void);

		void							addIndex(std::string index);
		void							addIndexes(std::set<std::string> indexes);
		std::set<std::string> const &	getIndexes(void) const;

	private:
		std::set<std::string>	_index;
};

class	UploadStore
{
	public:
		UploadStore(void);
		UploadStore(UploadStore const & src);
		UploadStore	&operator=(UploadStore const & right);
		~UploadStore(void);

		std::string const &	getPath(void) const;
		void				setPath(std::string const & path);

	private:
		std::string	_path;
};

#endif
