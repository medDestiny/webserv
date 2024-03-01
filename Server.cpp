/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:30:01 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 18:59:14 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::setListen(Listen listen) { _listen = listen; }

Listen const	&Server::getListen(Listen listen) const { return (_listen); }
