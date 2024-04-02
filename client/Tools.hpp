/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:05:31 by mmisskin          #+#    #+#             */
/*   Updated: 2024/04/01 22:51:15 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

size_t stringToInt(const std::string& str);
std::string intToString(size_t num);
size_t get_size_fd(std::string fileD);
bool isDirectory(const char* path);
bool isRegularFile(const char* path);
int hexToDec( std::string hexVal );
std::string stringToAscii( std::string const &str );
void urlDecoding( std::string &url );