/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:54:02 by mmisskin          #+#    #+#             */
/*   Updated: 2024/04/05 01:45:42 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Config.hpp"
#include "config/Parser.hpp"
#include "config/Location.hpp"
#include "server/Server.hpp"

#include<iostream>

int	main(int ac, char **av)
{

	signal(SIGPIPE, SIG_IGN);

	std::string	conf = DEFAULT_CONF;
	if (ac > 2)
		return (1);
	else if (ac == 2)
		conf = av[1];

	Config	config = Parser::importConfig(conf);
	if (!config.isValid())
		return (1);

	Server serv( config ); 
	std::cout << std::endl << BLUE << "--------> servers:" << RESET <<  std::endl; 
	serv.createServer(); 
}
