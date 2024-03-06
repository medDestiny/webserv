/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:52:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/06 20:10:55 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Directive.hpp"
#include <map>

class	Location;

namespace	Conf
{
	class	Server
	{
		public:
	
			void			setListen(Listen const & listen);
			Listen const	&getListen(void) const;
	
			void				setServerName(ServerName const & server_name);
			void				addServerName(std::vector<std::string> server_names);
			ServerName const &	getServerName(void) const;
	
			void				setErrorPage(ErrorPage const & error_page);
			void				addErrorPage(std::pair<std::string, std::string> const & error_page);
			ErrorPage const &	getErrorPage(void) const;
	
			void						setClientMaxBodySize(ClientMaxBodySize const & size);
			ClientMaxBodySize const &	getClientMaxBodySize(void) const;
	
			void				setReturn(Return const & _return);
			Return const &		getReturn(void) const;
	
			void				setRoot(Root const & root);
			Root const &		getRoot(void) const;
	
			void				setAutoIndex(AutoIndex const & autoindex);
			AutoIndex const &	getAutoIndex(void) const;
	
			void				setIndex(Index const & index);
			void				addIndex(std::vector<std::string> const & index);
			Index const &		getIndex(void) const;
	
			void				setUploadPath(UploadStore const & upload);
			UploadStore const &	getUploadPath(void) const;

			void										addLocation(std::pair<std::string, Location> location);
			void										setLocations(std::map<std::string, Location> const & locations);
			std::map<std::string, Location> const &		getLocations(void) const;

		protected:

			/* Server directives */
			Listen				_listen;
			ServerName			_server_name;
			ErrorPage			_error_page;
			ClientMaxBodySize	_client_max_body_size;
			Return				_return;
			Root				_root;
			AutoIndex			_autoindex;
			Index				_index;
			UploadStore			_upload_store;
	
		private:
			std::map<std::string, Location>	_locations;
	};
}

#endif
