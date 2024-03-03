NAME= webserv

SRC= Config.cpp \
	 Directive.cpp \
	 Parser.cpp \
	 Server.cpp \
	 Token.cpp \
	 main.cpp \
	 #server/Server.cpp

C= c++

C_FLAGS= -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address

HEADER=	Config.hpp \
		Directive.hpp \
		Location.hpp \
		Parser.hpp \
		Server.hpp \
		Token.hpp \
		#server/Server.hpp \
		server/Colors.hpp

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
