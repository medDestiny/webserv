/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:52:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 11:44:44 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Location.hpp"
#include "Directive.hpp"
#include <map>

class	Server
{
	private:
		std::map<std::string, Directive &>	_directives;
		/* std::map<std::string, Location>	_locations; */
};

#endif
