/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:57:29 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/29 02:19:44 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <string>
#include <vector>

class	Request;
class	Response;

class	Cgi
{
	public:
		Cgi(void);

		void				enable(void);
		bool				isSet(void) const;
		void				launch(void);
		bool				isStarted(void) const;

		void				setPid(pid_t const pid);
		void				setCgiTime(size_t time);
		void				setCgiStdErr(int stdErr);
		void				setFiles(std::string const & suffix);
		void				setScriptName(std::string const & name);
		void				setCwd(std::string const & cwd);
		void				setEnv(std::vector<std::string> const & env);
		void				setArgv(std::vector<std::string> const & argv);
		void				setPost(bool post);
		void				setReady(bool ready);

		bool				ready(void) const;
		pid_t				getPid(void) const;
		size_t				getCgiTime(void) const;
		int					getCgiStdErr(void) const;
		std::string const &	getCgiInFile(void) const;
		std::string const &	getCgiOutFile(void) const;

	private:
		bool						_isSet;
		bool						_started;
		pid_t						_pid;
		size_t						_cgiTime;
		int							_cgiStdErr;
		std::string					_cgiInFile;
		std::string					_cgiOutFile;

		bool						_post;
		bool						_ready;

		std::string					_scriptName;
		std::string					_cwd;
		std::vector<std::string>	_env;
		std::vector<std::string>	_argv;
};

int	monitorCgiProcess(Request & request, Response & response, int const sockfd);

#endif
