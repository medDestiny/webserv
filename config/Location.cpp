/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 18:54:21 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/12 14:16:17 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

void				Location::setLimitExcept(LimitExcept const & limit_except) { _allowed_methods = limit_except; }

LimitExcept const &	Location::getLimitExcept(void) const { return (_allowed_methods); }

void				Location::setCgiPass(CgiPass const & cgi_pass) { _cgi_pass = cgi_pass; }

CgiPass const &		Location::getCgiPass(void) const { return (_cgi_pass); }
