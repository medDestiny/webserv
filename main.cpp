/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:54:02 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/02 21:26:06 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Parser.hpp"
#include "server/Server.hpp"

#include<iostream>

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);

	Config	config = Parser::importConfig(av[1]);
	Server serv( config );
	
	std::cout << std::endl << BLUE << "--------> servers:" << RESET <<  std::endl;
	serv.createServer();

	if (!config.isValid())
	{
		std::cout << "ana ma3jebnich hh" << std::endl;
		return (1);
	}

	// config.print();

	// std::set<std::pair<std::string, std::string> > h;

	// h.insert( std::make_pair( "localhost", "8080" ) );
	// h.insert( std::make_pair( "0.0.0.0", "42" ) );
	// h.insert( std::make_pair( "127", "8080" ) );
	// h.insert( std::make_pair( "localhost", "8080" ) );

}
