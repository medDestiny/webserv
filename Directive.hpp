/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:21:11 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 12:02:21 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

#include <string>

class	Directive
{
	virtual	~Directive(void);
};

class	Listen : public Directive
{
	~Listen(void);

	private:
		std::string	_host;
		std::string	_port;
};

class	ServerName : public Directive
{
	~ServerName(void);

	private:
		
};

class	ErrorPage : public Directive
{
	~ErrorPage(void);
};

class	ClientMaxBodySize : public Directive
{
	~ClientMaxBodySize(void);
};

class	LimitExcept : public Directive
{
	~LimitExcept(void);
};

class	Return : public Directive
{
	~Return(void);
};

class	Root : public Directive
{
	~Root(void);
};

class	AutoIndex : public Directive
{
	~AutoIndex(void);
};

class	Index : public Directive
{
	~Index(void);
};

class	UploadStore : public Directive
{
	~UploadStore(void);
};

#endif
