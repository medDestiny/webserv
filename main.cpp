/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:54:02 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/03 18:22:41 by mmisskin         ###   ########.fr       */
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
	/* Server serv( config ); */
	
	/* std::cout << std::endl << BLUE << "--------> servers:" << RESET <<  std::endl; */
	/* serv.createServer(); */

	if (!config.isValid())
	{
		std::cout << "ana ma3jebnich hh" << std::endl;
		return (1);
	}

	Config	c1 = config;
	config.print();
	c1.print();

}
