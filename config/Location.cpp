/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 18:54:21 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/05 16:50:40 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

void				Location::setLimitExcept(LimitExcept const & limit_except) { _allowed_methods = limit_except; }

LimitExcept const &	Location::getLimitExcept(void) const { return (_allowed_methods); }
