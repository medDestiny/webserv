/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:46:56 by mmisskin          #+#    #+#             */
/*   Updated: 2024/04/05 01:46:19 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Server.hpp"
#include <vector>

class	Config
{
	public:
		Config(void);
		Config(Config const & src);
		Config	&operator=(Config const & right);

		/* Useful server lookup utility */
		Conf::Server							getServer(Conf::Server const & default_server, std::string server_name);

		/* Servers accessors */
		void									addServer(Conf::Server server);
		std::vector<Conf::Server> const &		getServers(void) const;

		/* Config validity */
		void									setAsValid(void);
		bool									isValid(void) const;

	private:
		std::vector<Conf::Server>	_servers;
		bool						_valid;
};

#endif
