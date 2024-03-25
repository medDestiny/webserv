/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:15:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/25 15:38:37 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../request/Request.hpp"
#include <unistd.h>

# define TMP "/tmp/.webservtmp"

void	Request::handleCgiRequest(std::map<std::string, Location>::iterator itLocation, std::string const & cgi, int sockfd)
{
	std::stringstream ss;
	ss << sockfd;
	this->tmpFile = TMP + ss.str();
	std::cout << this->tmpFile << std::endl;
	/* int	end[2]; */

	/* if (pipe(end) == -1) */
	/* 	std::cerr << "Error: cgi: " << strerror(errno) << std::endl; */

	pid_t	pid = fork();
	if (pid == -1)
		std::cerr << "Error: cgi: " << strerror(errno) << std::endl;
	if (!pid)
	{
		/* std::cout << this->path << std::endl; */
		/* std::cout << this->url << std::endl; */
		std::string	script = path.substr(path.rfind('/') + 1);
		/* std::string	cwd = itLocation->second.getRoot().getPath() + itLocation->first + path.substr(1, path.rfind('/')); */
		std::string	cwd = itLocation->second.getRoot().getPath() + itLocation->first;
		std::cout << cwd << std::endl;
		std::cout << script << std::endl;

		/* change the child's working directory */
		std::cout << chdir(cwd.c_str()) << std::endl;

		/* setup meta-variables */


		char const	**av = new char const*[2];
		av[0] = cgi.c_str();
		av[1] = script.c_str();
		
		int fd = open(this->tmpFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			std::cerr << RED << "file opening failed" << RESET << std::endl;
			exit(1);
		}
		std::cout << fd << std::endl;
		/* close(end[0]); */
		dup2(fd, STDOUT_FILENO);
		close(fd);
		/* close(end[1]); */

		execve(av[0], (char *const *)av, NULL);
		exit(0);
	}
	this->pid = pid;
	/* close(end[1]); */
}
