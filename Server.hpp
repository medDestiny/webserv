/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:52:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 22:50:35 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Directive.hpp"
#include <map>

class	Location;
class	Server
{
	public:

		void			setListen(Listen const & listen);
		Listen const	&getListen(void) const;

		void				setServerName(ServerName const & server_name);
		void				addServerName(std::set<std::string> server_names);
		ServerName const &	getServerName(void) const;

		void				setErrorPage(ErrorPage const & error_page);
		void				addErrorPage(std::pair<std::string, std::string> const & error_page);
		ErrorPage const &	getErrorPage(void) const;

		void						setClientMaxBodySize(ClientMaxBodySize const & size);
		ClientMaxBodySize const &	getClientMaxBodySize(ClientMaxBodySize const & size);

	protected:

		/* Server directives */
		Listen				_listen;
		ServerName			_server_name;
		ErrorPage			_error_page;
		ClientMaxBodySize	_client_max_body_size;
		/* Return				_return; */
		/* Root				_root; */
		/* AutoIndex			_autoindex; */
		/* Index				_index; */
		/* UploadStore			_upload_store; */

	private:
		/* std::map<std::string, Location>	_locations; */
};

#endif
