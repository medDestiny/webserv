/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoukhle <amoukhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:11:40 by amoukhle          #+#    #+#             */
/*   Updated: 2024/03/31 07:11:15 by amoukhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Script.hpp"

void	displayErrorPage() {
	std::string body;

	body = errorPage();
	
	std::cout << "Status: 500\r\n";
	std::cout << "Content-length: " + std::to_string( body.size() ) + "\r\n\r\n";
	std::cout << body;
}

void	displayLoginPage( std::string sessionId ) {
	std::string body;

	body = LoginPage();
	
	std::cout << "Content-length: " + std::to_string( body.size() ) + "\r\n";
	std::cout << "Set-Cookie: id=" + sessionId + "\r\n\r\n";
	std::cout << body;
}

void	displayWelcomePage( std::string login, std::string sessionId ) {
	std::string body;

	body = welcomePage(login);
	std::cout << "Content-length: " + std::to_string( body.size() ) + "\r\n";
	std::cout << "Set-Cookie: id=" + sessionId + "\r\n\r\n";
	std::cout << body;
}

void creatDataBase( std::string sessionId, std::string login, std::string password) {
	
	std::ofstream dataFd;
	dataFd.open("data.csv", std::ios::app);
	if (!dataFd.is_open()) {
		displayErrorPage();
		return ;
	}
	dataFd << "id,login,password\n";
	dataFd << sessionId << "," << login << "," << password << "\n";
	displayLoginPage(sessionId);
	dataFd.close();
}

std::string	checkIdIsExist( std::ifstream & dataFd, std::string sessionId ) {

	std::string line;
	// skip first line
	std::getline(dataFd, line);

	while (std::getline(dataFd, line)) {
		std::istringstream lineStream(line);
		std::string info;
		std::getline(lineStream, info, ',');
		if (info == sessionId) {
			std::getline(lineStream, info);
			return (info);
		}
	}
	return ("");
}

void addNewUser(std::string sessionId, std::string login, std::string password) {

	std::ofstream dataFd;
	dataFd.open("data.csv", std::ios::app);
	if (!dataFd.is_open()) {
		displayErrorPage();
		return ;
	}
	dataFd << sessionId << "," << login << "," << password << "\n";
	displayLoginPage(sessionId);
	dataFd.close();
}

int main(int ac, char **av, char **env) {

	std::string login = "miskin";
	std::string password = "44444";
	std::string flagValue = std::string( getenv("X_FLAG") );
	std::string sessionId = std::string( getenv("X_ID") );

	if (flagValue == "-1")
		displayLoginPage(sessionId);
	else {
		if (access("data.csv", F_OK) != 0) {
			creatDataBase(sessionId, login, password);
		}
		else {
			std::ifstream dataFd;
			dataFd.open("data.csv");
			if (!dataFd.is_open()) {
				displayErrorPage();
				return (0);
			}
			std::string info = checkIdIsExist(dataFd, sessionId);
			if (info.empty()) {
				
				addNewUser(sessionId, login, password);
			}
			else {
				std::istringstream infoStream(info);
				std::string data;
				std::getline(infoStream, data, ',');
				if (data == login) {
					std::getline(infoStream, data, ',');
					if (data == password)
						displayWelcomePage(login, sessionId);
					else
						displayLoginPage(sessionId);
				}
				else
					displayLoginPage(sessionId);
			}
			dataFd.close();
		}
	}
	return (0);
}