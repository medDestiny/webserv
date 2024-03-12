/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:21:11 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/12 14:04:23 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

#include <string>
#include <map>
#include <set>
#include <vector>

# define DEFAULT_HOST "0.0.0.0"
# define DEFAULT_PORT "8080"
# define DEFAULT_BODY_SIZE 1048576 // 1M in bytes
# define DEFAULT_UPLOAD_PATH "/"

class	Directive
{
	public:
		Directive(void);
		bool	empty(void) const;

	protected:
		bool	_is_set;
};

class	Listen : public Directive
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

		bool		operator==(Listen const & right) const;

	private:
		std::string	_host;
		std::string	_port;
};

class	ServerName : public Directive
{
	public:
		ServerName(void);
		ServerName(ServerName const & src);
		ServerName	&operator=(ServerName const & right);
		~ServerName(void);
    
		void								addHost(std::string host);
		void								addHosts(std::vector<std::string> hosts);
		void								setHosts(std::vector<std::string> hosts);
		std::vector<std::string> const &	getHosts(void) const;

	private:
		std::vector<std::string>	_hosts;
};

class	ErrorPage : public Directive
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

class	ClientMaxBodySize : public Directive
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

class	Return : public Directive
{
	public:
		Return(void);
		Return(Return const & src);
		Return	&operator=(Return const & right);
		~Return(void);

		int					getCode(void) const;
		void				setCode(int code);
		std::string	const & getUrl(void) const;
		void				setUrl(std::string const & url);

	private:
		int			_code;
		std::string _url;
};

class	Root : public Directive
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

class	AutoIndex : public Directive
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

class	Index : public Directive
{
	public:
		Index(void);
		Index(Index const & src);
		Index	&operator=(Index const & right);
		~Index(void);

		void								addIndex(std::string index);
		void								addIndexes(std::vector<std::string> indexes);
		std::vector<std::string> const &	getIndexes(void) const;

	private:
		std::vector<std::string>	_index;
};

class	UploadStore : public Directive
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

class	LimitExcept : public Directive
{
	public:
	LimitExcept(void);
	LimitExcept(LimitExcept const & src);
	LimitExcept	&operator=(LimitExcept const & right);
	~LimitExcept(void);

	void							setMethods(std::set<std::string> const & methods);
	std::set<std::string> const &	getMethods(void) const;

	private:
		std::set<std::string>	_methods;
};

class	CgiPass : public Directive 
{
	public:
		CgiPass(void);
		CgiPass(CgiPass const & src);
		CgiPass	&operator=(CgiPass const & right);
		~CgiPass(void);

		std::string const &	getCgi(void) const;
		void				setCgi(std::string const & cgi);

	private:
		std::string	_cgi;
};

#endif
