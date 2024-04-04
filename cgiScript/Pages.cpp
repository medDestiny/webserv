/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: del-yaag <del-yaag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:29:23 by amoukhle          #+#    #+#             */
/*   Updated: 2024/04/04 03:11:33 by del-yaag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Script.hpp"

std::string LoginPage( void ) {
	std::string html_code = "<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"    <title>Login</title>\n"
		"    <style>\n"
		"        :root{\n"
		"            --primary-100:#1F3A5F;\n"
		"            --primary-300:#acc2ef;\n"
		"            --accent-100:#3D5A80;\n"
		"            --accent-200:#cee8ff;\n"
		"            --text-100:#FFFFFF;\n"
		"            --bg-100:#0F1C2E;\n"
		"            --bg-200:#1f2b3e;\n"
		"        }\n"
		"        *{ padding: 0; margin: 0; box-sizing: border-box; font-family: Arial, sans-serif; }\n"
		"        body{ display: flex; justify-content: center; align-items: center; height: 100vh; background-color: var(--bg-100); }\n"
		"        .container{ display: flex; flex-direction: column; width: 700px; padding: 20px; background-color: var(--bg-200); }\n"
		"        @media (max-width: 768px) { .container{ width: 95%; } }\n"
		"        .container h2{ text-align: center; color: var(--text-100); margin-bottom: 30px; }\n"
		"        /* .container form{ margin-top: 30px; } */\n"
		"        .container form div label{ display: block; margin-bottom: 10px; color: var(--accent-200); }\n"
		"        .container form div input{ padding: 10px; width: 100%; background-color: var(--bg-200);\n"
		"            border: 1px solid var(--accent-100); outline: none; color: var(--accent-200); margin-bottom: 20px; }\n"
		"        .container form button{ padding: 10px; width: 100%; border: none; cursor: pointer; margin-top: 15px;\n"
		"            font-weight: bold; font-size: 18px; background-color: var(--primary-100); color: var(--primary-300); }\n"
		"    </style>\n"
		"</head>\n"
		"<body>\n"
		"    <div class=\"container\">\n"
		"        <h2>Login</h2>\n"
		"        <form action=\"/a.out\" method=\"post\" enctype=\"multipart/form-data\">\n"
		"            <div class=\"input-group\">\n"
		"                <label for=\"username\">Username:</label>\n"
		"                <input type=\"text\" id=\"login\" name=\"login\" required>\n"
		"            </div>\n"
		"            <div class=\"input-group\">\n"
		"                <label for=\"password\">Password:</label>\n"
		"                <input type=\"password\" id=\"password\" name=\"password\" required>\n"
		"            </div>\n"
		"            <button type=\"submit\">Login</button>\n"
		"        </form>\n"
		"    </div>\n"
		"</body>\n"
		"</html>\n";
	return (html_code);
}

std::string welcomePage( std::string login ) {
	std::string welcome_page = "<!DOCTYPE html>\n"
				"<html lang=\"en\">\n"
				"<head>\n"
				"    <title>Document</title>\n"
				"    <style>\n"
				"        :root{ --accent-100:#3D5A80; --text-100:#FFFFFF; --bg-100:#0F1C2E; }\n"
				"        *{ padding: 0; margin: 0; box-sizing: border-box; font-family: Arial, sans-serif; }\n"
				"        body{ display: flex; justify-content: center; align-items: center; height: 100vh; background-color: var(--bg-100); }\n"
				"        .container{ width: 700px; padding: 20px; text-align: center; }\n"
				"        .container h1{ color: var(--text-100); margin-bottom: 15px; }\n"
				"        .container p{ font-size: 20px; color: var(--accent-100); }\n"
				"    </style>\n"
				"</head>\n"
				"<body>\n"
				"    <div class=\"container\">\n"
				"        <h1>Welcome, " + login + ".</h1>\n"
				"        <p>Your are successfully logged in.</p>\n"
				"    </div>\n"
				"</body>\n"
				"</html>\n";
	return (welcome_page);
}