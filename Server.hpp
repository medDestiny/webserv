/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:52:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 18:58:23 by mmisskin         ###   ########.fr       */
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

		void			setListen(Listen listen);
		Listen const	&getListen(Listen listen) const;

	protected:

		/* Server directives */
		Listen				_listen;
		/* ServerName			_server_name; */
		/* ErrorPage			_error_page; */
		/* ClientMaxBodySize	_client_max_body_size; */
		/* Return				_return; */
		/* Root				_root; */
		/* AutoIndex			_autoindex; */
		/* Index				_index; */
		/* UploadStore			_upload_store; */

	private:
		/* std::map<std::string, Location>	_locations; */
};

#endif
