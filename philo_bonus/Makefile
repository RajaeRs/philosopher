SRC = main_bonus.c utils_bonus.c get_data_bonus.c

NAME = philo_bonus

HEADER = philo_bonus.h

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

.PHONY: all clean fclean re