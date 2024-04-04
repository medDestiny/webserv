/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:52:25 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/31 04:47:05 by mmisskin         ###   ########.fr       */
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

			/* Easier access to locations */
			std::map<std::string, Location>::iterator	getLocation(std::string const & path);
	
			/* Accessors to the server directives */
			void										setListen(Listen const & listen);
			Listen const								&getListen(void) const;
			Listen										&getListen(void);
	
			void										setServerName(ServerName const & server_name);
			void										addServerName(std::vector<std::string> server_names);
			ServerName const &							getServerName(void) const;
			ServerName &								getServerName(void);
	
			void										setErrorPage(ErrorPage const & error_page);
			void										addErrorPage(std::pair<std::string, std::string> const & error_page);
			ErrorPage const &							getErrorPage(void) const;
			ErrorPage &									getErrorPage(void);
	
			void										setClientMaxBodySize(ClientMaxBodySize const & size);
			ClientMaxBodySize const &					getClientMaxBodySize(void) const;
			ClientMaxBodySize &							getClientMaxBodySize(void);
	
			void										setReturn(Return const & _return);
			Return const &								getReturn(void) const;
			Return &									getReturn(void);
	
			void										setRoot(Root const & root);
			Root const &								getRoot(void) const;
			Root &										getRoot(void);
	
			void										setAutoIndex(AutoIndex const & autoindex);
			AutoIndex const &							getAutoIndex(void) const;
			AutoIndex &									getAutoIndex(void);
	
			void										setIndex(Index const & index);
			void										addIndex(std::vector<std::string> const & index);
			Index const &								getIndex(void) const;
			Index &										getIndex(void);
	
			void										setUploadPath(UploadStore const & upload);
			UploadStore const &							getUploadPath(void) const;
			UploadStore &								getUploadPath(void);

			void										addLocation(std::pair<std::string, Location> location);
			void										setLocations(std::map<std::string, Location> const & locations);
			std::map<std::string, Location> const &		getLocations(void) const;
			std::map<std::string, Location> &			getLocations(void);

		protected:
    
			/* Common directives for server and location */
			ErrorPage			_error_page;
			ClientMaxBodySize	_client_max_body_size;
			Return				_return;
			Root				_root;
			AutoIndex			_autoindex;
			Index				_index;
			UploadStore			_upload_store;
	
		private:

			/* Server specific directives */
			Listen				_listen;
			ServerName			_server_name;

			/* Server locations */
			std::map<std::string, Location>	_locations;
	};
}

#endif
