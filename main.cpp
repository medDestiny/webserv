/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:54:02 by mmisskin          #+#    #+#             */
/*   Updated: 2024/04/04 01:43:29 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Config.hpp"
#include "config/Parser.hpp"
#include "config/Location.hpp"
#include "server/Server.hpp"

#include<iostream>

int	main(int ac, char **av)
{
	// signal(SIGPIPE, SIG_IGN);
	if (ac != 2)
		return (1);

	Config	config = Parser::importConfig(av[1]);
	Server serv( config ); 
	
	// config.print();
	if (!config.isValid())
		return (1);

	std::cout << std::endl << BLUE << "--------> servers:" << RESET <<  std::endl; 
	serv.createServer(); 
}
