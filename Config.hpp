/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:46:56 by mmisskin          #+#    #+#             */
/*   Updated: 2024/02/28 18:01:16 by mmisskin         ###   ########.fr       */
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

		void	setAsValid(void);
		bool	isValid(void) const;

	private:
		bool				_valid;
		std::vector<Server>	_servers;
};

#endif
