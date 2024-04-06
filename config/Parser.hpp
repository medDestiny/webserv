/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:43:33 by mmisskin          #+#    #+#             */
/*   Updated: 2024/04/05 01:48:02 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# define DEFAULT_CONF "default.conf"

#include <exception>
#include <string>

class	Config;

class	Parser
{
	public:
		static Config	importConfig(std::string const & path);

	class	Error : public std::exception
	{
		public:
			Error(std::string error) throw();
			Error(std::string error, std::string token, size_t line) throw();
			~Error(void) throw();

			char const	*what() const throw();

		private:
			std::string	_error;
	};
};

#endif
