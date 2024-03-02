/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:46:56 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/02 12:11:06 by mmisskin         ###   ########.fr       */
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

		void							setAsValid(void);
		void							addServer(Conf::Server server);
		std::vector<Conf::Server> const &		getServers(void) const;
		bool							isValid(void) const;

	private:
		bool				_valid;
		std::vector<Conf::Server>	_servers;
};

#endif
