/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:43:33 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/06 20:10:12 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <exception>
#include <string>

class	Config;

class	Parser
{
	public:
		static Config	importConfig(char const *path);

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
