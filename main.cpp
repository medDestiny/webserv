/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:54:02 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 21:04:28 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Parser.hpp"

#include<iostream>

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);

	Config	config = Parser::importConfig(av[1]);
	
	if (!config.isValid())
	{
		std::cout << "ana ma3jebnich hh" << std::endl;
		return (1);
	}

	config.print();
}
