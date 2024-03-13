/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:54:10 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/13 16:51:15 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Server.hpp"

class	Location : public Conf::Server
{
	public:

		void				setLimitExcept(LimitExcept const & limit_except);
		LimitExcept const &	getLimitExcept(void) const;

		void				addCgiPass(std::pair<std::string, std::string> const & cgi);
		CgiPass const &		getCgiPass(void) const;

	private:

		/* Location specific directive */
		LimitExcept	_allowed_methods;
		CgiPass		_cgi_pass;
};

#endif
