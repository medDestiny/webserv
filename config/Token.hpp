/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:03:06 by mmisskin          #+#    #+#             */
/*   Updated: 2024/02/29 15:18:34 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

#include <string>

enum	Types
{
	OPEN_BR,
	CLOSE_BR,
	SEMICOLON,
	SERVER,
	LOCATION,
	DIRECTIVE
};

class	Token
{
	public:
		Token(Types type, std::string content);

		Types		type(void) const;
		std::string	content(void) const;

	private:
		Token(void);

		Types		_type;
		std::string	_content;
};

#endif
