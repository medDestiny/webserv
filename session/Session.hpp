/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:12:06 by del-yaag          #+#    #+#             */
/*   Updated: 2024/04/04 03:13:03 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <map>

#define TIME 120

class Session {
	
	private:
		static std::map<std::string, std::time_t> sessions;

	public:
		static void addSession( std::string const &id );
		static bool findSessionId( std::string const &id );
		static void deleteSessionIdTimeOut( void );
};