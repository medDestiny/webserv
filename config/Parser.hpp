/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:43:33 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/01 10:06:55 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <exception>

class	Config;

class	Parser
{
	public:
		static Config	importConfig(char const *path);

	/* Implement errors (undone) */
	class	Error : public std::exception
	{
		public:
			char const	*what() const throw() { return ("error"); }
	};
};

#endif
