NAME = miniRT

CC = gcc

LIST =	main.c	src/atod.c	src/display.c	src/calculate.c src/errors.c  src/utils_free.c\
		src/sphere.c src/ray_tracing.c src/vectors.c src/calculate_s.c   src/camera.c\
		src/plane.c	src/cylinder2.c src/parsing.c

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
	rm -f ./libft/gnl/*.o
	rm -f ./libft/gnl/*.a

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re bonus
