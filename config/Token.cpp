/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:09:26 by mmisskin          #+#    #+#             */
/*   Updated: 2024/02/29 11:10:38 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(Types type, std::string content)
{
	_type = type;
	_content = content;
}

Types		Token::type(void) const
{
	return (_type);
}

std::string	Token::content(void) const
{
	return (_content);
}
