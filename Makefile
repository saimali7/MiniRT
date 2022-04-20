NAME = miniRT

CC = gcc

LIST =	main.c

OBJ = $(LIST:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) :	$(OBJ)
			$(MAKE) -C ./libft
			$(MAKE) -C ./mlx
			$(CC) $(LIST) $(CFLAGS) $(OBJS) -L ./mlx -l mlx -framework OpenGL -Ofast -framework AppKit -L ./libft -l ft -lm -o $(NAME)

%.o : %.c
	$(CC) -Wall -Wextra -Werror -Imlx -c $< -o $@

bonus : $(NAME)

clean:
	rm -rf $(OBJ)
	rm -f ./mlx/*.a
	rm -f ./libft/*.o
	rm -f ./libft/*.a

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re bonus