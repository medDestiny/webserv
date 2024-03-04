/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:45:59 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/04 20:03:58 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Config.hpp"
#include "Token.hpp"
#include "Location.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace	Conf;

bool	isNumber(std::string const & num)
{
	for (size_t i = 0; num[i]; i++)
	{
		if (!std::isdigit(num[i]))
			return (false);
	}
	return (true);
}

Listen	ParseListen(std::vector<Token> & Tokens)
{
	std::string	host;
	std::string	port;
	Listen		listen;

	Tokens.erase(Tokens.begin()); // delete listen token

	Token	token = Tokens.front();
	if (token.type() == DIRECTIVE)
	{
		size_t	separator;

		separator = Tokens.front().content().find(':', 0);
		if (separator != std::string::npos)
		{
			/* warning: additional checks for port and host may be required */
			host = token.content().substr(0, separator);
			port = token.content().substr(separator + 1);

			if (!isNumber(port))
				throw Parser::Error();

			listen.setHost(host);
			listen.setPort(port);
		}
		else if (std::isalpha(token.content()[0]) || token.content().find('.', 0) != std::string::npos)
			listen.setHost(token.content());
		else
		{
			if (!isNumber(token.content()))
				throw Parser::Error();

			listen.setPort(token.content());
		}

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

std::set<std::string>	ParseServerName(std::vector<Token> & Tokens)
{
	std::set<std::string>	hosts;

	Tokens.erase(Tokens.begin()); // delete server_name token

	if (Tokens.front().type() == DIRECTIVE)
	{
		while (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		{
			hosts.insert(Tokens.front().content());
			Tokens.erase(Tokens.begin()); // delete host token after adding it
		}
		if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error();
	}
	else
		throw Parser::Error();

	return (hosts);
}

std::pair<std::string, std::string>	ParseErrorPage(std::vector<Token> & Tokens)
{
	std::pair<std::string, std::string>	error;
	std::string							code;
	std::string							path;

	Tokens.erase(Tokens.begin()); // delete error_page token

	if (Tokens.front().type() == DIRECTIVE)
	{
		code = Tokens.front().content();

		if (!isNumber(code))
			throw Parser::Error();

		error.first = code;
		Tokens.erase(Tokens.begin()); // delete code token
	}
	else
		throw Parser::Error();

	if (Tokens.front().type() == DIRECTIVE)
	{
		path = Tokens.front().content();
		error.second = path;
		Tokens.erase(Tokens.begin()); // delete path token
	}
	else
		throw Parser::Error();

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error();

	return (error);
}

ClientMaxBodySize	ParseClientMaxBodySize(std::vector<Token> & Tokens)
{
	ClientMaxBodySize	max_size;

	Tokens.erase(Tokens.begin()); // delete client_max_body_size token

	if (Tokens.front().type() == DIRECTIVE)
	{
		std::string	token = Tokens.front().content();
		size_t		unit = token.find_first_not_of("0123456789", 0);

		if (unit == std::string::npos) // case of a size without a unit (client... 1024;)
			throw Parser::Error();

		std::stringstream	ss;
		size_t				size;

		ss << token.substr(0, unit);
		if (!(ss >> size) || ss.peek() != EOF)
			throw Parser::Error();
		token = token.substr(unit);

		/* Convert the size to bytes */
		if (token == "k" || token == "K")
			size *= 1024;
		else if (token == "m" || token == "M")
			size *= 1048576;
		else if (token == "g" || token == "G")
			size *= 1073741824;
		else
			throw Parser::Error();

		max_size.setSize(size);
		Tokens.erase(Tokens.begin()); // delete client_max_body_size unit token
	}
	else
		throw Parser::Error();

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error();

	return (max_size);
}

Return	ParseReturn(std::vector<Token> & Tokens)
{
	Return	_return;

	Tokens.erase(Tokens.begin()); // delete return token

	if (Tokens.front().type() == DIRECTIVE)
	{
		std::stringstream	ss;
		int					code;

		ss << Tokens.front().content();
		if (!(ss >> code) || ss.peek() != EOF)
			throw Parser::Error();

		/* warning: some additionnal checks on the code validity needed */

		_return.setCode(code);
		Tokens.erase(Tokens.begin()); // delete return code token
	}
	else
		throw Parser::Error();

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error();

	return (_return);
}

Root	ParseRoot(std::vector<Token> & Tokens)
{
	Root	root;

	Tokens.erase(Tokens.begin()); // delete root token

	if (Tokens.front().type() == DIRECTIVE)
	{
		/* warning: some additionnal checks on the path validity needed */

		root.setPath(Tokens.front().content());
		Tokens.erase(Tokens.begin()); // delete root path token
	}
	else
		throw Parser::Error();

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error();

	return (root);
}

AutoIndex	ParseAutoIndex(std::vector<Token> & Tokens)
{
	AutoIndex	autoindex;

	Tokens.erase(Tokens.begin()); // delete autoindex token

	if (Tokens.front().type() == DIRECTIVE)
	{
		std::string	option = Tokens.front().content();

		if (option == "on")
			autoindex.setToggle(true);
		else if (option == "off")
			autoindex.setToggle(false);
		else
			throw Parser::Error();

		Tokens.erase(Tokens.begin()); // delete option token
	}
	else
		throw Parser::Error();

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error();

	return (autoindex);
}

std::set<std::string>	ParseIndex(std::vector<Token> & Tokens)
{
	std::set<std::string>	indexes;

	Tokens.erase(Tokens.begin()); // delete index token

	if (Tokens.front().type() == DIRECTIVE)
	{
		while (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		{
			indexes.insert(Tokens.front().content());
			Tokens.erase(Tokens.begin()); // delete each index token after adding it
		}
		if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error();
	}
	else
		throw Parser::Error();

	return (indexes);
}

UploadStore	ParseUploadStore(std::vector<Token> & Tokens)
{
	UploadStore	upload;

	Tokens.erase(Tokens.begin()); // delete upload_store token

	if (Tokens.front().type() == DIRECTIVE)
	{
		/* warning: some additionnal checks on the path validity needed */

		upload.setPath(Tokens.front().content());
		Tokens.erase(Tokens.begin()); // delete upload path token
	}
	else
		throw Parser::Error();

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error();

	return (upload);
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

std::pair<std::string, Location>	ParseLocation(Server const & server, std::vector<Token> & Tokens)
{
	std::pair<std::string, Location>	location("", server);
	std::stack<Types>					brackets;

	/* Check for location path */
	if (Tokens.empty() || Tokens.front().type() != DIRECTIVE)
		throw Parser::Error();
	else
	{
		location.first = Tokens.front().content();
		Tokens.erase(Tokens.begin());
	}

	/* Check opening bracket */
	if (Tokens.empty() || !CheckBrackets(brackets, Tokens[0]))
		throw Parser::Error();
	else
		Tokens.erase(Tokens.begin());


	while (!Tokens.empty() && Tokens.front().type() != CLOSE_BR)
	{
		/* add directive to the directives list */
		if (Tokens.front().content() == "error_page")
			location.second.addErrorPage(ParseErrorPage(Tokens));
		else if (Tokens.front().content() == "client_max_body_size")
			location.second.setClientMaxBodySize(ParseClientMaxBodySize(Tokens));
		else if (Tokens.front().content() == "return")
			location.second.setReturn(ParseReturn(Tokens));
		else if (Tokens.front().content() == "root")
			location.second.setRoot(ParseRoot(Tokens));
		else if (Tokens.front().content() == "autoindex")
			location.second.setAutoIndex(ParseAutoIndex(Tokens));
		else if (Tokens.front().content() == "index")
			location.second.addIndex(ParseIndex(Tokens));
		else if (Tokens.front().content() == "upload_store")
			location.second.setUploadPath(ParseUploadStore(Tokens));
		else
		{
			std::cout << "Location: unknown directive: " << Tokens.front().content() << std::endl;
			throw Parser::Error();
		}
	}

	/* Check closing bracket */
	if (Tokens.empty() || !CheckBrackets(brackets, Tokens[0]))
		throw Parser::Error();
	else
		Tokens.erase(Tokens.begin());

	return (location);
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
		/* std::cout << "Curr token ---> " << Tokens.front().content() << std::endl; */
		if (Tokens.front().type() == LOCATION)
		{
			std::pair<std::string, Location>	location;
			std::map<std::string, Location>		locations;
			Tokens.erase(Tokens.begin());	// delete the location token

			location = ParseLocation(server, Tokens);
			locations = server.getLocations();
			if (locations.find(location.first) != locations.end())
				throw Parser::Error();		// Duplicate location
			server.addLocation(location);
		}
		else
		{
			/* add directive to the directives list */
			if (Tokens.front().content() == "listen")
				server.setListen(ParseListen(Tokens));
			else if (Tokens.front().content() == "server_name")
				server.addServerName(ParseServerName(Tokens));
			else if (Tokens.front().content() == "error_page")
				server.addErrorPage(ParseErrorPage(Tokens));
			else if (Tokens.front().content() == "client_max_body_size")
				server.setClientMaxBodySize(ParseClientMaxBodySize(Tokens));
			else if (Tokens.front().content() == "return")
				server.setReturn(ParseReturn(Tokens));
			else if (Tokens.front().content() == "root")
				server.setRoot(ParseRoot(Tokens));
			else if (Tokens.front().content() == "autoindex")
				server.setAutoIndex(ParseAutoIndex(Tokens));
			else if (Tokens.front().content() == "index")
				server.addIndex(ParseIndex(Tokens));
			else if (Tokens.front().content() == "upload_store")
				server.setUploadPath(ParseUploadStore(Tokens));
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

	if (line.find("server_name", 0) == std::string::npos
		&&line.substr(pos, std::strlen("server")) == "server")			// Add the 'server' block Token
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
