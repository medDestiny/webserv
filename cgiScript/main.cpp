/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 03:54:38 by del-yaag          #+#    #+#             */
/*   Updated: 2024/03/31 06:39:57 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiScript.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

size_t get_size_fd(std::string fileD) {
	struct stat fileStat;

	if (lstat(fileD.c_str(), &fileStat) != 0) {
		std::cout << "error getSize\n";
		return (-1);
	}
	return (fileStat.st_size);
}

int main( void ) {

    // std::string body = "----------------------------239821545604467274478286\r\nContent-Disposition: form-data; name=\"login\"\r\n\r\npppppp\r\n----------------------------239821545604467274478286\r\nContent-Disposition: form-data; name=\"password\"\r\n\r\ndel-yaag\r\n----------------------------239821545604467274478286--\r\n";
    ParseData parser;
    std::string body;
    // int fd;
    
    // fd = open( "/tmp/.cgiInput5", O_RDONLY, 0644 );
    // if ( fd == -1 ) {

    //     std::cout << "error to open file" << std::endl;
    //     return 1;
    // }
    // size_t size = 0;
    // size = get_size_fd( "/tmp/.cgiInput5" );
    // char buffer[size] = {0};
    // ssize_t bytes = read( fd, buffer, size );
    // if ( bytes == -1 ) {

    //     std::cout << "error to read from file" << std::endl;
    //     return 2;
    // }

    std::ifstream infile;
    
    infile.open( "/tmp/.cgiInput5" );

    std::string buffer;
    std::string tmp;
    while (getline(infile, tmp))
        buffer += tmp + '\n';

    parser.setBody( buffer );

    parser.getStartEndBoundaries( parser.getBody() );
    if ( !parser.parseBody() ) {
        
        std::cout << "error" << std::endl;
        return 2;
    }
    std::cout << "login:\t\t" << parser.getLogin() << std::endl;
    std::cout << "password:\t" << parser.getPassword() << std::endl;
    return 0;
}