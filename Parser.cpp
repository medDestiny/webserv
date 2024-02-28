/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:45:59 by mmisskin          #+#    #+#             */
/*   Updated: 2024/02/28 18:55:28 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Config.hpp"
#include <fstream>
#include <iostream>

/* Server	ParseServer() */
/* { */

/* } */

Config	Parser::importConfig(char const *path)
{
	Config			config;
	std::ifstream	configFile(path);

	if (configFile.is_open() == false)
		return (config);

	std::string	line;
	while (std::getline(configFile, line))
	{
		if (line.find_first_not_of(" \t", 0) != std::string::npos)
			std::cout << line << std::endl;
	}

	config.setAsValid();
	return (config);
}
