/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:05:58 by amoukhle          #+#    #+#             */
/*   Updated: 2024/04/04 00:47:35 by del-yaag         ###   ########.fr       */
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
		// std::cout << "error getSize\n";
		return (-1);
	}
	return (fileStat.st_size);
}

bool isDirectory(const char* path) {
    struct stat pathStat;
    if (stat(path, &pathStat) != 0) {
		// std::cout << "error isDirectory\n";
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

void urlDecoding( std::string &url ) {
	
	char decimal;
	std::string s;
	std::string chunkedUrl;
	std::size_t found = url.find("%");
	while (found!=std::string::npos) {
		
		chunkedUrl = url.substr(0, found);
		s = url.substr( found + 1, 2 );
		decimal = hexToDec( s );
		chunkedUrl += decimal;
		url.erase(0, found + 3);
		url = chunkedUrl.append( url );
		found = url.find("%");
	}
}

int hexToDec( std::string hexVal ) {

    int len = hexVal.size();
    int base = 1;
    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--) {

        if (hexVal[i] >= '0' && hexVal[i] <= '9') {

            dec_val += (int(hexVal[i]) - 48) * base;
            base = base * 16;
            
        } else if (hexVal[i] >= 'a' && hexVal[i] <= 'f') {

            dec_val += (int(hexVal[i]) - 87) * base;
            base = base * 16;
        } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {

            dec_val += (int(hexVal[i]) - 55) * base;
            base = base * 16;
        } 
    }
    return dec_val;
}

std::string stringToAscii( std::string const &str ) {

    std::string ascii;

    for ( size_t i = 0; i < str.size(); i++ ) {

        std::stringstream stream;
        stream << static_cast<int>( str[i] );
        ascii += stream.str();
    }

    return ascii;
}