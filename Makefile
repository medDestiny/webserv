NAME= webserv

SRC= config/Config.cpp \
	 config/Directive.cpp \
	 config/Location.cpp \
	 config/Parser.cpp \
	 config/Server.cpp \
	 config/Token.cpp \
	 server/Server.cpp \
	 client/Client.cpp \
	 client/Tools.cpp \
	 response/Response.cpp \
	 response/MimeType.cpp \
	 request/Request.cpp \
	 request/GetMethod.cpp \
	 response/postMethod.cpp \
	 request/postMethod.cpp \
	 cgi/cgi.cpp \
	 session/Session.cpp \
	 main.cpp

C= c++

C_FLAGS= -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

HEADER=	config/Config.hpp \
		config/Directive.hpp \
		config/Location.hpp \
		config/Parser.hpp \
		config/Server.hpp \
		config/Token.hpp \
		server/Server.hpp \
		server/Colors.hpp \
		client/Client.hpp \
		client/Tools.hpp \
	 	response/Response.hpp \
	 	request/Request.hpp \
		session/Session.hpp \
	 	cgi/Cgi.hpp

OBJ= $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(C) $(C_FLAGS) $^ -o $@

%.o: %.cpp $(HEADER)
	$(C) $(C_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
