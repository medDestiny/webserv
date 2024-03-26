/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:15:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/26 02:57:57 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../request/Request.hpp"
#include <unistd.h>

# define TMP "/tmp/.webservtmp"

std::string	getIdentifier(std::string header)
{
	/* remove the : at the end (Header':') */
	header.pop_back();

	for (size_t i = 0; header[i]; i++)
	{
		if (header[i] >= 'a' && header[i] <= 'z')
			header[i] = std::toupper(header[i]);
		else if (header[i] == '-')
			header[i] = '_';
	}
	header = "HTTP_" + header;

	return (header);
}

char **buildEnv(std::map<std::string, std::string> & req, std::string const & method, std::string const & protocol, std::string const & url)
{
	/* setup meta-variables */
	std::vector<std::string>	environment;
	std::string	queryString;
	environment.push_back("REQUEST_METHOD=" + method);
	environment.push_back("SERVER_PROTOCOL=" + protocol);
	if (url.find('?') != std::string::npos)
	{
		environment.push_back("QUERY_STRING=" + url.substr(url.find('?') + 1));
		environment.push_back("PATH_INFO=" + url.substr(0, url.find('?')));
	}
	else
	{
		environment.push_back("QUERY_STRING");
		environment.push_back("PATH_INFO=" + url);
	}

	std::string	entry;
	for (std::map<std::string, std::string>::iterator it = req.begin(); it != req.end(); it++)
	{
		if (it->second.empty())
			entry = getIdentifier(it->first);
		else
			entry = getIdentifier(it->first) + "=" + it->second;
		environment.push_back(entry);
	}

	/* convert the vector to a (char **) */
	size_t	size = environment.size() + 1;
	char **env = new char *[size];
	env[size - 1] = NULL;
	for (size_t i = 0; i < environment.size(); i++)
	{
		env[i] = new char [environment[i].length() + 1];
		env[i][environment[i].length()] = 0;
		environment[i].copy(env[i], environment[i].length());
	}
	return (env);
}

std::string	getScriptName(std::string path)
{
	size_t	pos;
	size_t	len;
	size_t	start;

	/* find the first extension */
	pos = path.find('.');

	/* go back till u hit a / */
	start = path.rfind('/', pos) + 1;

	/* calculate the length ]x, x[ or ]x, end] (x = {/, ?}) */
	len = path.find_first_of("/?", start) - start;

	return (path.substr(start, len));
}

void	Request::handleCgiRequest(std::map<std::string, Location>::iterator itLocation, std::string const & cgi, int sockfd)
{
	std::stringstream ss;

	ss << sockfd;
	this->tmpFile = TMP + ss.str();
	std::cout << this->tmpFile << std::endl;

	int	end[2];
	if (pipe(end) == -1)
		std::cerr << "Error: cgi pipe: " << strerror(errno) << std::endl;

	if (fcntl(end[0], F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		close(end[0]);
		close(end[1]);
	}
	if (fcntl(end[1], F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		close(end[0]);
		close(end[1]);
	}

	pid_t	pid = fork();
	if (pid == -1)
		std::cerr << "Error: cgi: " << strerror(errno) << std::endl;
	if (!pid)
	{
		/* std::cout << this->path << std::endl; */
		/* std::cout << this->url << std::endl; */
		/* std::cout << cgi << std::endl; */
		/* std::cout << path.substr(path.find('.')) << std::endl; */
		std::string	script = getScriptName(path);
		/* std::string	cwd = itLocation->second.getRoot().getPath() + itLocation->first + path.substr(1, path.rfind('/')); */
		std::string	cwd = itLocation->second.getRoot().getPath() + itLocation->first;
		/* std::cout << cwd << std::endl; */
		/* std::cout << script << std::endl; */

		/* change the child's working directory */
		chdir(cwd.c_str());

		/* set the script's environment */
		char **env = buildEnv(this->linesRequest, this->method, this->httpVersion, this->url);

		char **av;
		if (cgi.empty())
		{
			av = new char *[2];
			av[0] = (char *)("./" + script).c_str();
			av[1] = NULL;
		}
		else
		{
			av = new char *[3];
			av[0] = (char *)cgi.c_str();
			av[1] = (char *)script.c_str();
			av[2] = NULL;
		}
		
		/* int in = open("/tmp/.test", O_RDONLY); */
		/* if (in == -1) */
		/* { */
		/* 	std::cerr << RED << "file opening failed" << RESET << std::endl; */
		/* 	exit(1); */
		/* } */
		/* dup2(in, STDIN_FILENO); */
		/* close(in); */

		/* setup and change child's standart output */
		int fd = open(this->tmpFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			std::cerr << RED << "file opening failed" << RESET << std::endl;
			exit(1);
		}
		std::cout << fd << std::endl;
		dup2(fd, STDOUT_FILENO);
		close(fd);

		/* std::cerr << "before" << std::endl; */
		close(end[0]);
		dup2(end[1], STDERR_FILENO);
		close(end[1]);
		/* std::cerr << "after" << std::endl; */

		execve(av[0], av, env);
		exit(0);
	}

	/* close the write end since we're not gonna use it */
	close(end[1]);

	this->pid = pid;
	this->cgiTime = std::time(NULL);
	this->cgiStdErr = end[0];
}
