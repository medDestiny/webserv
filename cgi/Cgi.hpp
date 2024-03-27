/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:57:29 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/27 02:40:54 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <unistd.h>
#include <string>

class	Cgi
{
	public:
		Cgi(void);

		void				enable(void);
		bool				isSet(void) const;

		void				setPid(pid_t const pid);
		void				setCgiTime(size_t time);
		void				setCgiTmpFile(std::string const & tmp);
		void				setCgiStdErr(int stdErr);
		void				setFileSuffix(std::string const & suffix);
		pid_t				getPid(void) const;
		size_t				getCgiTime(void) const;
		int					getCgiStdErr(void) const;
		std::string const &	getCgiTmpFile(void) const;

	private:
		bool		_isSet;
		pid_t		_pid;
		size_t		_cgiTime;
		int			_cgiStdErr;
		std::string	_cgiTmpFile;
		std::string	_cgiTmpFileSuffix;
};

#endif
