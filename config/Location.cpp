/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 18:54:21 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/31 02:17:19 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

void				Location::setLimitExcept(LimitExcept const & limit_except) { _allowed_methods = limit_except; }

LimitExcept const &	Location::getLimitExcept(void) const { return (_allowed_methods); }
LimitExcept &		Location::getLimitExcept(void) { return (_allowed_methods); }

void				Location::addCgiPass(std::pair<std::string, std::string> const & cgi) { _cgi_pass.addCgi(cgi); }

CgiPass const &		Location::getCgiPass(void) const { return (_cgi_pass); }
CgiPass &			Location::getCgiPass(void) { return (_cgi_pass); }
