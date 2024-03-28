/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoukhle <amoukhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:05:58 by amoukhle          #+#    #+#             */
/*   Updated: 2024/03/19 23:05:59 by amoukhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/stat.h>

size_t stringToInt(const std::string& str) {
	std::istringstream iss(str);
	size_t result;
	iss >> result;
	return result;
}

std::string intToString(size_t num) {
	std::ostringstream oss;
	oss << num;
	return oss.str();
}

size_t get_size_fd(std::string fileD) {
	struct stat fileStat;

	if (lstat(fileD.c_str(), &fileStat) != 0) {
		std::cout << "error getSize\n";
		return (-1);
	}
	return (fileStat.st_size);
}

bool isDirectory(const char* path) {
    struct stat pathStat;
    if (stat(path, &pathStat) != 0) {
		std::cout << "error isDirectory\n";
        return false;
    }
    return S_ISDIR(pathStat.st_mode);
}

bool isRegularFile(const char* path) {
    struct stat pathStat;
    if (stat(path, &pathStat) != 0) {
		std::cout << "error isFile\n";
        return false;
    }
    return S_ISREG(pathStat.st_mode);
}