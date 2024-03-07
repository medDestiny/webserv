/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:46:56 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/07 11:08:31 by mmisskin         ###   ########.fr       */
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

		void	print(void) const;		// for debugging

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
