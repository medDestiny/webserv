/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:45:59 by mmisskin          #+#    #+#             */
/*   Updated: 2024/02/29 18:44:36 by mmisskin         ###   ########.fr       */
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

/* bool	ParseServer(std::ifstream & configFile, std::string line, Config & config) */
/* { */
/* 	std::stack<char>	brackets; */

/* 	if (line.find_first_of("{}", 0) != std::string::npos) */
/* 		if (CheckBrackets(brackets, line) == false) */
/* 			std::cout << "brackets: " << line << std::endl; */

/* 	while (std::getline(configFile, line)) */
/* 	{ */
/* 		std::cout << "server :" << line << std::endl; */
/* 		if (line.find("}", 0) != std::string::npos) */
/* 			break ; */
/* 	} */
/* 	return (true); */
/* } */

bool	ParseLocation(std::vector<Token> & Tokens)
{
	std::stack<char>	brackets;

	std::cout << "Location context: first token: " << Tokens[0].content() << std::endl;
	if (Tokens.size() == 0 || Tokens[0].type() != DIRECTIVE)
	{
		std::cout << "Location: unexpected token: " << Tokens[0].content() << std::endl;
		return (false);
	}
	if (Tokens.size() == 0 || Tokens[1].type() != OPEN_BR)
	{
		std::cout << "Location: unexpected token: " << Tokens[1].content() << std::endl;
		return (false);
	}
	return (true);
}

bool	ParseServer(std::vector<Token> & Tokens)
{
	std::stack<char>	brackets;

	std::cout << "Server context" << std::endl;
	if (Tokens.size() == 0 || Tokens[0].type() != OPEN_BR)
	{
		std::cout << "Server: unexpected token: " << Tokens[0].content() << std::endl;
		return (false);
	}
	else
		Tokens.erase(Tokens.begin());

	for (size_t i = 0; i < Tokens.size(); i++)
	{
		if (Tokens[i].type() == LOCATION)
		{
			Tokens.erase(Tokens.begin());
			if (!ParseLocation(Tokens))
				return (false);
		}
	}
	return (true);
}

bool	Parse(std::vector<Token> & Tokens)
{
	for (size_t i = 0; i < Tokens.size(); i++)
	{
		if (Tokens[i].type() == SERVER)
		{
			Tokens.erase(Tokens.begin());
			if (!ParseServer(Tokens))
				return (false);
		}
		else
		{
			std::cout << "[main context] error " << Tokens[i].content() << std::endl;
			return (false);
		}
	}
	return (true);
}

size_t	TokenizeString(std::vector<Token> & Tokens, std::string const line, size_t pos)
{
	size_t	len;


	if (line.substr(pos, std::strlen("server")) == "server")			// Add the 'server' block Token
	{
		Tokens.push_back(Token(SERVER, "server"));
		return (std::strlen("server"));
	}
	else if (line.substr(pos, std::strlen("location")) == "location")	// Add the 'location' block Token
	{
		Tokens.push_back(Token(LOCATION, "location"));
		return (std::strlen("location"));
	}

	/* Add other string tokens */
	len = 0;
	while (line[pos + len] && std::string(";{} \t").find(line[pos + len], 0) == std::string::npos)
		len++;
	Tokens.push_back(Token(DIRECTIVE, line.substr(pos, len)));

	return (len);
}

/*
 * Eliminates whitespaces
 * Splits the file into tokens
 */
std::vector<Token>	Tokenize(std::ifstream & configFile)
{
	std::vector<Token>	Tokens;
	std::string			line;
	size_t				len;

	while (std::getline(configFile, line))
	{
		/* Skip empty lines and comments */
		len = line.find_first_not_of(" \t", 0);
		if (len == std::string::npos || line[len] == '#')
			continue ;

		for (size_t i = 0; line[i]; i++)
		{
			if (line[i] == ' ' || line[i] == '\t')
				continue ;
			else if (line[i] == '{')
				Tokens.push_back(Token(OPEN_BR, "{"));
			else if (line[i] == '}')
				Tokens.push_back(Token(CLOSE_BR, "}"));
			else if (line[i] == ';')
			{
				len = line.find_last_of(';') - i + 1;
				Tokens.push_back(Token(SEMICOLON, line.substr(i, len)));
				i += len - 1;
			}
			else
				i += TokenizeString(Tokens, line, i) - 1;
		}
	}
	return (Tokens);
}

Config	Parser::importConfig(char const *path)
{
	Config			config;
	std::ifstream	configFile(path);

	if (configFile.is_open() == false)
		return (config);

	std::vector<Token>	Tokens = Tokenize(configFile);

	for (size_t i = 0; i < Tokens.size(); i++)
	{
		std::cout << Tokens[i].content() << std::endl;
	}

	if (!Parse(Tokens))
		return (config);

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
