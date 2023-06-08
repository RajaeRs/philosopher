SRC = main.c starting.c controling.c utils.c get_data.c

NAME = philo

HEADER = philo.h

OBJ = $(SRC:.c=.o)

CC = cc

FLAGE = -Wall -Wextra -Werror -pthread 

all : $(NAME)

%.o: %.c 
	$(CC) -c $< -o $@

$(NAME) : $(OBJ) $(HEADER)
		$(CC) $(FLAGE) $(SRC) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean $(NAME)

# .PHONY: all clean fclean re