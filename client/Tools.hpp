/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisskin <mmisskin@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:05:31 by mmisskin          #+#    #+#             */
/*   Updated: 2024/03/29 01:06:46 by mmisskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

size_t stringToInt(const std::string& str);
std::string intToString(size_t num);
size_t get_size_fd(std::string fileD);
bool isDirectory(const char* path);
bool isRegularFile(const char* path);
