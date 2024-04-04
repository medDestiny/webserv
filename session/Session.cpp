/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:15:35 by del-yaag          #+#    #+#             */
/*   Updated: 2024/04/04 03:13:00 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"
#include "../server/Colors.hpp"

std::map<std::string, std::time_t> Session::sessions;


void Session::addSession( std::string const &id ) {

	Session::sessions[id] = std::time( NULL );
	// std::cout << "count: " << Session::sessions.size() << std::endl;
}

bool Session::findSessionId( std::string const &id ) {

	std::map<std::string, std::time_t>::iterator it = Session::sessions.find( id );
	if ( it != Session::sessions.end() )
		return true;
	return false;
}

void Session::deleteSessionIdTimeOut( void ) {

		
	std::map<std::string, std::time_t>::iterator it = Session::sessions.begin();
	std::time_t now = std::time( NULL );
	
	for ( ; it != Session::sessions.end(); it++ ) {

		if ( now - it->second >= TIME ) {
			
			Session::sessions.erase( it->first );
			std::cout << BLUE << "\t-> session has been deleted!" << RESET << std::endl;
			if ( sessions.size() == 0 )
				break;
			it = Session::sessions.begin();
		}
	}
}