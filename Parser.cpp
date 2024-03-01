/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:45:59 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 19:00:43 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Config.hpp"
#include "Token.hpp"
#include <fstream>
#include <iostream>

Listen	ParseListen(std::vector<Token> & Tokens)
{
	std::string	host;
	std::string	port;
	Listen	listen;

	Tokens.erase(Tokens.begin()); // delete listen token

	Token	token = Tokens.front();
	if (token.type() == DIRECTIVE)
	{
		size_t	separator;

		separator = Tokens.front().content().find(':', 0);
		if (separator != std::string::npos)
		{
			/* warning: additional checks for port and host required */
			host = token.content().substr(0, separator);
			port = token.content().substr(separator + 1);
		}
		else
			listen.setPort(token.content());

		Tokens.erase(Tokens.begin()); // delete option

		if (Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error();
	}
	else
		throw Parser::Error();

	return (listen);
}

bool	CheckBrackets(std::stack<Types> & brackets, Token const & token)
{
	switch (token.type())
	{
		case OPEN_BR:
		{
			if (brackets.empty())
				brackets.push(OPEN_BR);
			else
				return (false);
			break ;
		}
		case CLOSE_BR:
		{
			if (!brackets.empty() && brackets.top() == OPEN_BR)
				brackets.pop();
			else
				return (false);
			break ;
		}
		default:
			return (false);
	}
	return (true);
}

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

Server	ParseServer(std::vector<Token> & Tokens)
{
	Server				server;
	std::stack<Types>	brackets;

	/* Check opening bracket */
	if (Tokens.empty() || !CheckBrackets(brackets, Tokens[0]))
		throw Parser::Error();
	else
		Tokens.erase(Tokens.begin());

	/* Check server directives and location blocks */
	while (!Tokens.empty() && Tokens.front().type() != CLOSE_BR)
	{
		if (Tokens.front().type() == LOCATION)
		{
			Tokens.erase(Tokens.begin());
			if (!ParseLocation(Tokens))
				throw Parser::Error();
		}
		else
		{
			/* add directive to the directives list */
			if (Tokens.front().content() == "listen")
			{
				std::cout << "Server: listen: ";
				/* server.setListen(ParseListen(Tokens)); */
				Listen l = ParseListen(Tokens);
				std::cout << l.getHost() << " : " << l.getPort() << std::endl;
			}
			else if (Tokens.front().content() == "server_name")
			{

			}
			else if (Tokens.front().content() == "error_page")
			{

			}
			else if (Tokens.front().content() == "client_max_body_size")
			{

			}
			else if (Tokens.front().content() == "return")
			{

			}
			else if (Tokens.front().content() == "root")
			{

			}
			else if (Tokens.front().content() == "autoindex")
			{

			}
			else if (Tokens.front().content() == "index")
			{

			}
			else if (Tokens.front().content() == "upload_store")
			{

			}
			else
			{
				std::cout << "Server: unknown directive: " << Tokens.front().content() << std::endl;
				throw Parser::Error();
			}
		}
	}

	/* Check closing bracket */
	if (Tokens.empty() || !CheckBrackets(brackets, Tokens[0]))
		throw Parser::Error();
	else
		Tokens.erase(Tokens.begin());

	return (server);
}

bool	Parse(Config & config, std::vector<Token> & Tokens)
{
	while (!Tokens.empty())
	{
		if (Tokens.front().type() == SERVER)
		{
			Tokens.erase(Tokens.begin());
			try
			{
				config.addServer(ParseServer(Tokens));
			}
			catch (Parser::Error & e)
			{
				/* std::cerr << e.what() << std::endl; */
				std::cerr << "Error: Parse exception" << std::endl;
				return (false);
			}
		}
		else
		{
			std::cout << "[main context] error: unexpected token: " << Tokens.front().content() << std::endl;
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

	/* for (size_t i = 0; i < Tokens.size(); i++) */
	/* { */
	/* 	std::cout << Tokens[i].content() << std::endl; */
	/* } */

	if (!Parse(config, Tokens))
		return (config);

	config.setAsValid();
	return (config);
}
