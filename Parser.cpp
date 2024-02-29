/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:45:59 by mmisskin          #+#    #+#             */
/*   Updated: 2024/02/29 12:57:05 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Config.hpp"
#include "Token.hpp"
#include <fstream>
#include <iostream>

bool	CheckBrackets(std::stack<char> & brackets, std::string line)
{
	for (size_t i = line.find_first_not_of("server", 0); line[i]; i++)
	{
		switch (line[i])
		{
			case '}':
			{
				if (!brackets.empty() && brackets.top() == '{')
					brackets.pop();
				else
					return (false);
				break ;
			}
			case '{':
			{
				if (brackets.empty() == false)
					return (false);
				brackets.push(line[i]);
				break ;
			}
			case ' ':
				break ;
			case '\t':
				break ;
			default:
				return (false);
		}
	}
	return (true);
}

bool	ParseServer(std::ifstream & configFile, std::string line, Config & config)
{
	std::stack<char>	brackets;

	if (line.find_first_of("{}", 0) != std::string::npos)
		if (CheckBrackets(brackets, line) == false)
			std::cout << "brackets: " << line << std::endl;

	while (std::getline(configFile, line))
	{
		std::cout << "server :" << line << std::endl;
		if (line.find("}", 0) != std::string::npos)
			break ;
	}
	return (true);
}

bool	Tokenize(std::ifstream & configFile, std::vector<Token> & Tokens)
{
	std::string			line;

	while (std::getline(configFile, line))
	{
		/* Skip comments */
		if (line[0] == '#')
			continue ;
		for (size_t i = 0; line[i]; i++)
		{
			if (line[i] == ' ' || line[i] == '\t')
				continue ;
			else if (std::isalpha(line[i]))
			{
				if (line.substr(i, std::strlen("server")) == "server")
				{
					Tokens.push_back(Token(SERVER, "server"));
					i += std::strlen("server") - 1;
				}
				else if (line.substr(i, std::strlen("location")) == "location")
				{
					Tokens.push_back(Token(LOCATION, "location"));
					i += std::strlen("location") - 1;
				}
			}
			else if (line[i] == '{')
				Tokens.push_back(Token(OPEN_BR, "{"));
			else if (line[i] == '}')
				Tokens.push_back(Token(CLOSE_BR, "}"));
			else if (line[i] == ';')
			{
				size_t	len = line.find_last_of(';') - i + 1;
				Tokens.push_back(Token(SEMICOLON, line.substr(i, len)));
				i += len - 1;
			}
			else
				return (false);
			/* std::cout << "next " << i << " \'" << line[i] << "\'" << std::endl; */
		}
	}
	return (true);
}

Config	Parser::importConfig(char const *path)
{
	Config			config;
	std::ifstream	configFile(path);

	if (configFile.is_open() == false)
		return (config);

	std::vector<Token>	Tokens;
	if (!Tokenize(configFile, Tokens))
		return (config);

	for (size_t i = 0; i < Tokens.size(); i++)
	{
		std::cout << Tokens[i].content() << std::endl;
	}
	/* std::string	line; */
	/* while (std::getline(configFile, line)) */
	/* { */
	/* 	std::cout << "line: " << line << std::endl; */
	/* 	if (line.find("server", 0) != std::string::npos) */
	/* 	{ */
	/* 		if (ParseServer(configFile, line, config) == false) */
	/* 			return (config); */
	/* 	} */
	/* 	else if (line.find_first_not_of(" \t", 0) != std::string::npos) */
	/* 		return (config); */
	/* } */

	config.setAsValid();
	return (config);
}
