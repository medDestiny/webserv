/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoukhle <amoukhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:45:59 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/31 05:16:15 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Config.hpp"
#include "Token.hpp"
#include "Location.hpp"
#include "../server/Colors.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace	Conf;

Parser::Error::Error(std::string error) throw()
{
	_error = error;
}

Parser::Error::~Error(void) throw() {}

Parser::Error::Error(std::string error, std::string token, size_t line) throw()
{
	std::stringstream ss;
	ss << line;
	_error = error + token + " (line: " + ss.str() + ")";
}

char const	*Parser::Error::what() const throw()
{
	return (_error.c_str());
}

bool	isNumber(std::string const & num)
{
	for (size_t i = 0; num[i]; i++)
	{
		if (!std::isdigit(num[i]))
			return (false);
	}
	return (true);
}

bool	isValidPort(std::string port)
{
	std::stringstream	ss;
	size_t				portNumber;

	ss << port;
	if (!(ss >> portNumber) || ss.peek() != EOF)
		return (false);

	if (portNumber < 1024 || portNumber > 65535)
		return (false);
	return (true);
}

bool	isValidErrorCode(std::string code)
{
	std::stringstream	ss;
	size_t				errorCode;

	ss << code;
	if (!(ss >> errorCode) || ss.peek() != EOF)
		return (false);

	if (errorCode < 300 || errorCode > 599)
		return (false);
	return (true);
}

void	ParseListen(Listen & listen, std::vector<Token> & Tokens)
{
	std::string	host;
	std::string	port;

	if (!listen.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	Tokens.erase(Tokens.begin()); // delete listen token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		Token	token = Tokens.front();
		size_t	separator;

		separator = Tokens.front().content().find(':', 0);
		if (separator != std::string::npos)
		{
			host = token.content().substr(0, separator);
			port = token.content().substr(separator + 1);

			if (port.empty())
				throw Parser::Error("empty port number: ", port, token.line());
			else if (host.empty())
				throw Parser::Error("empty host: ", host, token.line());
			else if (!isNumber(port) || !isValidPort(port))
				throw Parser::Error("invalid port number: ", port, token.line());

			listen.setHost(host);
			listen.setPort(port);
		}
		else if (std::isalpha(token.content()[0]) || token.content().find('.', 0) != std::string::npos)
			listen.setHost(token.content());
		else
		{
			if (!isNumber(token.content()) || !isValidPort(token.content()))
				throw Parser::Error("invalid port number: ", token.content(), token.line());

			listen.setPort(token.content());
		}

		Tokens.erase(Tokens.begin()); // delete option

		if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error("missing semicolon at end of directive: listen");
	}
	else
		throw Parser::Error("invalid number of arguments in listen");
}

void	ParseServerName(ServerName & server_name, std::vector<Token> & Tokens)
{
	Tokens.erase(Tokens.begin()); // delete server_name token

	if (Tokens.empty() || Tokens.front().type() == DIRECTIVE)
	{
		while (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		{
			server_name.addHost(Tokens.front().content());
			Tokens.erase(Tokens.begin()); // delete host token after adding it
		}

		if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error("missing semicolon at end of directive: server_name");
	}
	else
		throw Parser::Error("invalid number of arguments in server_name");
}

void	ParseErrorPage(ErrorPage	error_pages, std::vector<Token> & Tokens)
{
	std::string		code;
	std::string		path;

	Tokens.erase(Tokens.begin()); // delete error_page token

	for (std::vector<Token>::iterator it = Tokens.begin(); it != Tokens.end() && it + 1 != Tokens.end(); it++)
	{
		if ((it + 1)->type() == SEMICOLON)
		{
			path = it->content();
			Tokens.erase(it); // delete path token
			break ;
		}
	}
	
	if (Tokens.empty() || Tokens.front().type() != DIRECTIVE)
		throw Parser::Error("invalid number of arguments in error_page");

	while (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		code = Tokens.front().content();

		if (!isNumber(code) || !isValidErrorCode(code))
			throw Parser::Error("invalid error code: ", code, Tokens.front().line());

		/* Protect against duplicate error codes with the same path */
		if (error_pages.getErrorPages().find(code) != error_pages.getErrorPages().end()
			&& error_pages.getErrorPages().find(code)->second != path)
			throw Parser::Error("conflicting error_page for error code: ", code, Tokens.front().line());

		error_pages.addErrorPage(std::make_pair(code, path));
		Tokens.erase(Tokens.begin()); // delete code token
	}

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: error_page");
}

void	ParseClientMaxBodySize(ClientMaxBodySize & max_size, std::vector<Token> & Tokens)
{
	if (!max_size.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	Tokens.erase(Tokens.begin()); // delete client_max_body_size token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		std::string	token = Tokens.front().content();
		size_t		unit = token.find_first_not_of("0123456789", 0);

		if (unit == std::string::npos) // case of a size without a unit (client... 1024;)
			throw Parser::Error("missing unit for ", "client_max_body_size", Tokens.front().line());

		std::stringstream	ss;
		size_t				size;

		ss << token.substr(0, unit);
		if (!(ss >> size) || ss.peek() != EOF)
			throw Parser::Error("invalid body size ", token, Tokens.front().line());
		token = token.substr(unit);

		/* Convert the size to bytes */
		if (token == "k" || token == "K")
			size *= 1024;
		else if (token == "m" || token == "M")
			size *= 1048576;
		else if (token == "g" || token == "G")
			size *= 1073741824;
		else
			throw Parser::Error("invalid unit in ", "client_max_body_size", Tokens.front().line());

		max_size.setSize(size);
		Tokens.erase(Tokens.begin()); // delete client_max_body_size value token
	}
	else
		throw Parser::Error("invalid number of arguments in client_max_body_size");

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: client_max_body_size");
}

void	ParseReturn(Return & _return, std::vector<Token> & Tokens)
{
	if (!_return.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	Tokens.erase(Tokens.begin()); // delete return token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		std::stringstream	ss;
		int					code;

		ss << Tokens.front().content();
		if (!(ss >> code) || ss.peek() != EOF)
			throw Parser::Error("invalid return code: ", Tokens.front().content(), Tokens.front().line());

		if (code < 300 || code > 399)
			throw Parser::Error("invalid http redirection code: ", Tokens.front().content(), Tokens.front().line()); // invalid redirection code
		else if (code != 301 && code != 302 && code != 303 && code != 307 && code != 308)
			throw Parser::Error("unsupported http redirection code: ", Tokens.front().content(), Tokens.front().line()); // unsupported redirection code

		_return.setCode(code);
		Tokens.erase(Tokens.begin()); // delete return code token
		if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		{
			_return.setUrl(Tokens.front().content());
			Tokens.erase(Tokens.begin()); // delete return url token
		}
		else
			throw Parser::Error("missing url for return");
	}
	else
		throw Parser::Error("invalid number of arguments in return");

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: return");
}

void	ParseRoot(Root & root, std::vector<Token> & Tokens)
{
	if (!root.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	Tokens.erase(Tokens.begin()); // delete root token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		std::string	path = Tokens.front().content();
		size_t		slash = path.find_last_not_of('/');

		path = path.substr(0, slash + 1);
		root.setPath(path);
		Tokens.erase(Tokens.begin()); // delete root path token
	}
	else
		throw Parser::Error("invalid number of arguments in root");

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: root");
}

void	ParseAutoIndex(AutoIndex & autoindex, std::vector<Token> & Tokens)
{
	if (!autoindex.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	Tokens.erase(Tokens.begin()); // delete autoindex token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		std::string	option = Tokens.front().content();

		if (option == "on")
			autoindex.setToggle(true);
		else if (option == "off")
			autoindex.setToggle(false);
		else
			throw Parser::Error("invalid option: ", option, Tokens.front().line());

		Tokens.erase(Tokens.begin()); // delete option token
	}
	else
		throw Parser::Error("invalid number of arguments in autoindex");

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: autoindex");
}

void	ParseIndex(Index & indexes, std::vector<Token> & Tokens)
{
	Tokens.erase(Tokens.begin()); // delete index token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		while (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		{
			indexes.addIndex(Tokens.front().content());
			Tokens.erase(Tokens.begin()); // delete each index token after adding it
		}

		if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error("missing semicolon at end of directive: index");
	}
	else
		throw Parser::Error("invalid number of arguments in index");
}

void	ParseUploadStore(UploadStore & upload, std::vector<Token> & Tokens)
{
	if (!upload.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	Tokens.erase(Tokens.begin()); // delete upload_store token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		std::string	path = Tokens.front().content();
		size_t		slash = path.find_last_not_of('/');

		path = path.substr(0, slash + 1);
		upload.setPath(path);
		Tokens.erase(Tokens.begin()); // delete upload path token
	}
	else
		throw Parser::Error("invalid number of arguments in upload_store");

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: upload_store");
}

void	ParseLimitExcept(LimitExcept & limit_except, std::vector<Token> & Tokens)
{
	if (!limit_except.empty())
		throw Parser::Error("duplicate directive: ", Tokens.front().content(), Tokens.front().line());

	std::set<std::string>	allowed_methods;

	Tokens.erase(Tokens.begin()); // delete limit_except token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		std::string	token;
		while (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		{
			token = Tokens.front().content();

			/* Detect unknown methods */
			if (token != "GET" && token != "POST" && token != "DELETE")
				throw Parser::Error("unknown http method ", token, Tokens.front().line());

			allowed_methods.insert(Tokens.front().content());
			Tokens.erase(Tokens.begin()); // delete method token after adding it
		}

		if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
			Tokens.erase(Tokens.begin()); // delete semicolon
		else
			throw Parser::Error("missing semicolon at end of directive: limit_except");
	}
	else
		throw Parser::Error("invalid number of arguments in limit_except");

	limit_except.setMethods(allowed_methods);
}

void	ParseCgiPass(CgiPass & cgi_pass, std::vector<Token> & Tokens)
{
	std::string extension;
	std::string cgi;

	Tokens.erase(Tokens.begin()); // delete cgi_pass token

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		extension = Tokens.front().content();
		if (extension[0] != '.')
			throw Parser::Error("invalid file extension: ", extension, Tokens.front().line());
		else if (cgi_pass.found(extension))
			throw Parser::Error("duplicate cgi for existing extension: ", extension, Tokens.front().line());
		Tokens.erase(Tokens.begin()); // delete cgi extension token
	}
	else
		throw Parser::Error("invalid number of arguments in cgi_pass");

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
	{
		cgi = Tokens.front().content();
		Tokens.erase(Tokens.begin()); // delete cgi path token
	}

	if (!Tokens.empty() && Tokens.front().type() == DIRECTIVE)
		throw Parser::Error("invalid number of arguments in cgi_pass");

	if (!Tokens.empty() && Tokens.front().type() == SEMICOLON)
		Tokens.erase(Tokens.begin()); // delete semicolon
	else
		throw Parser::Error("missing semicolon at end of directive: cgi_pass");

	cgi_pass.addCgi(std::make_pair(extension, cgi));
}

void	fillServerLocations(Server & server)
{
	std::map<std::string, Location> locations = server.getLocations();

	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->second.getErrorPage().empty())
			it->second.setErrorPage(server.getErrorPage());
		if (it->second.getRoot().empty())
			it->second.setRoot(server.getRoot());
		if (it->second.getClientMaxBodySize().empty())
			it->second.setClientMaxBodySize(server.getClientMaxBodySize());
		if (it->second.getReturn().empty())
			it->second.setReturn(server.getReturn());
		if (it->second.getAutoIndex().empty())
			it->second.setAutoIndex(server.getAutoIndex());
		if (it->second.getIndex().empty())
			it->second.setIndex(server.getIndex());
		if (it->second.getUploadPath().empty())
			it->second.setUploadPath(server.getUploadPath());
	}
	server.setLocations(locations);
}

std::pair<std::string, Location>	ParseLocation(std::vector<Token> & Tokens)
{
	std::pair<std::string, Location>	location;

	/* Check for location path */
	if (Tokens.empty() || Tokens.front().type() != DIRECTIVE)
		throw Parser::Error("missing path for location block");
	else
	{
		location.first = Tokens.front().content();
		Tokens.erase(Tokens.begin());
	}

	/* Check opening bracket */
	if (Tokens.empty() || Tokens.front().type() != OPEN_BR)
		throw Parser::Error("missing opening bracket for location " + location.first);
	else
		Tokens.erase(Tokens.begin());


	while (!Tokens.empty() && Tokens.front().type() != CLOSE_BR)
	{
		/* add directive to the directives list */
		if (Tokens.front().content() == "error_page")
			ParseErrorPage(location.second.getErrorPage(), Tokens);
		else if (Tokens.front().content() == "client_max_body_size")
			ParseClientMaxBodySize(location.second.getClientMaxBodySize(), Tokens);
		else if (Tokens.front().content() == "return")
			ParseReturn(location.second.getReturn(), Tokens);
		else if (Tokens.front().content() == "root")
			ParseRoot(location.second.getRoot(), Tokens);
		else if (Tokens.front().content() == "autoindex")
			ParseAutoIndex(location.second.getAutoIndex(), Tokens);
		else if (Tokens.front().content() == "index")
			ParseIndex(location.second.getIndex(), Tokens);
		else if (Tokens.front().content() == "upload_store")
			ParseUploadStore(location.second.getUploadPath(), Tokens);
		else if (Tokens.front().content() == "limit_except")
			ParseLimitExcept(location.second.getLimitExcept(), Tokens);
		else if (Tokens.front().content() == "cgi_pass")
			ParseCgiPass(location.second.getCgiPass(), Tokens);
		else
			throw Parser::Error("unknown directive in location context: ", Tokens.front().content(), Tokens.front().line());
	}

	/* Check closing bracket */
	if (Tokens.empty() || Tokens.front().type() != CLOSE_BR)
		throw Parser::Error("missing closing bracket for location " + location.first);
	else
		Tokens.erase(Tokens.begin());

	return (location);
}

Server	ParseServer(std::vector<Token> & Tokens)
{
	Server								server;
	std::pair<std::string, Location>	location;
	std::map<std::string, Location>		locations;

	/* Check opening bracket */
	if (Tokens.empty() || Tokens.front().type() != OPEN_BR)
		throw Parser::Error("missing opening bracket for server");
	else
		Tokens.erase(Tokens.begin());

	/* Check server directives and location blocks */
	while (!Tokens.empty() && Tokens.front().type() != CLOSE_BR)
	{
		if (Tokens.front().type() == LOCATION)
		{
			Tokens.erase(Tokens.begin());	// delete the location token

			location = ParseLocation(Tokens);
			locations = server.getLocations();
			if (locations.find(location.first) != locations.end())
				throw Parser::Error("duplicate location: " + location.first);
			server.addLocation(location);
		}
		else
		{
			/* add directive to the directives list */
			if (Tokens.front().content() == "listen")
				ParseListen(server.getListen(), Tokens);
			else if (Tokens.front().content() == "server_name")
				ParseServerName(server.getServerName(), Tokens);
			else if (Tokens.front().content() == "error_page")
				ParseErrorPage(server.getErrorPage(), Tokens);
			else if (Tokens.front().content() == "client_max_body_size")
				ParseClientMaxBodySize(server.getClientMaxBodySize(), Tokens);
			else if (Tokens.front().content() == "return")
				ParseReturn(server.getReturn(), Tokens);
			else if (Tokens.front().content() == "root")
				ParseRoot(server.getRoot(), Tokens);
			else if (Tokens.front().content() == "autoindex")
				ParseAutoIndex(server.getAutoIndex(), Tokens);
			else if (Tokens.front().content() == "index")
				ParseIndex(server.getIndex(), Tokens);
			else if (Tokens.front().content() == "upload_store")
				ParseUploadStore(server.getUploadPath(), Tokens);
			else
				throw Parser::Error("unknown directive in server context: ", Tokens.front().content(), Tokens.front().line());
		}
	}

	/* Make sure locations inherit server's config */
	fillServerLocations(server);

	/* Check closing bracket */
	if (Tokens.empty() || Tokens.front().type() != CLOSE_BR)
		throw Parser::Error("missing closing bracket for server");
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
				std::cerr << RED << "[emerg] " << e.what() << RESET << std::endl;
				return (false);
			}
		}
		else
		{
			std::cerr << "unknown directive in main context: " 
			<< Tokens.front().content() << " (line: "
			<< Tokens.front().line() << ")"<< std::endl;
			return (false);
		}
	}
	return (true);
}

size_t	TokenizeString(std::vector<Token> & Tokens, std::string const line, size_t count, size_t pos)
{
	size_t	len;

	if (line.substr(pos, line.find_first_of(";{}\t ", pos)) == "server") // Add the 'server' block Token
	{
		Tokens.push_back(Token(SERVER, "server", count));
		return (std::strlen("server"));
	}
	else if (line.substr(pos, std::strlen("location")) == "location")	// Add the 'location' block Token
	{
		Tokens.push_back(Token(LOCATION, "location", count));
		return (std::strlen("location"));
	}

	/* Add other string tokens */
	len = 0;
	while (line[pos + len] && std::string(";{} \t").find(line[pos + len], 0) == std::string::npos)
		len++;
	Tokens.push_back(Token(DIRECTIVE, line.substr(pos, len), count));

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
	size_t				count = 0; // line counter
	size_t				len;

	while (std::getline(configFile, line) && ++count)
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
				Tokens.push_back(Token(OPEN_BR, "{", count));
			else if (line[i] == '}')
				Tokens.push_back(Token(CLOSE_BR, "}", count));
			else if (line[i] == ';')
			{
				len = line.find_first_not_of(';', i);
				if (len == std::string::npos)
					len = line.length();
			   	len -= i;
				Tokens.push_back(Token(SEMICOLON, line.substr(i, len), count));
				i += len - 1;
			}
			else
				i += TokenizeString(Tokens, line, count, i) - 1;
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
	/* for (std::vector<Token>::iterator it = Tokens.begin(); it != Tokens.end(); it++) */
	/* { */
	/* 	std::cout << it->content() << std::endl; */
	/* } */
	/* exit(0); */
	if (!Parse(config, Tokens))
		return (config);

	config.setAsValid();
	return (config);
}
