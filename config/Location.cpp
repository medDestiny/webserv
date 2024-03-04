/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 18:54:21 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/04 20:00:18 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void) {}
Location::Location(Conf::Server const & src)
{
	_error_page = src.getErrorPage();
	_client_max_body_size = src.getClientMaxBodySize();
	_return = src.getReturn(); // might or not be correct to copy this here
	_root = src.getRoot();
	_autoindex = src.getAutoIndex();
	_index = src.getIndex();
	_upload_store = src.getUploadPath();
}

void				Location::setLimitExcept(LimitExcept const & limit_except) { _allowed_methods = limit_except; }

void				Location::allowMethod() {}

LimitExcept const &	Location::getLimitExcept(void) const { return (_allowed_methods); }
