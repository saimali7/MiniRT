NAME = miniRT

CC = gcc

LIST =	src/main.c	src/geometry/plane.c	src/geometry/sphere.c	src/geometry/paraboloid.c	\
		src/geometry/vectors_p1.c	src/geometry/vectors_p2.c  	src/geometry/cylinder.c		src/parsing/parsing.c	\
		src/parsing/parse_sphere.c	src/parsing/parse_plane.c	src/parsing/parse_cylinder.c	src/parsing/parse_paraboloid.c	\
		src/parsing/parse_ambient_light.c 	src/parsing/parse_camera.c 	src/parsing/parsing_utils.c  src/parsing/parse_rt_line.c\
		src/render/calculate.c src/render/trace_ray.c	src/render/render_utils.c\
		src/utils/errors.c	src/utils/atod.c	src/utils/utils_free.c	\
		src/render/camera.c		src/render/chessboard.c	src/render/display.c	\
		src/render/render_shapes.c			src/render/render_shape_utils.c


OBJ = $(LIST:.c=.o)

HEADERS = inc/MiniRt.h inc/Minilibx.h inc/Vector.h

CFLAGS = -Wall -Wextra -Werror

GREEN = "\033[32m"
RESET = "\033[0m"
CUT = "\033[K"
DOWN = "\033[B"
UP = "\033[A"
SAVE = "\033[s"

all : $(NAME)

$(NAME) :	$(OBJ)
			@ar rc $@ $(OBJ)
			@$(MAKE) -C ./libft
			@$(MAKE) -C ./mlx
			@$(CC) $(LIST) $(CFLAGS) $(OBJS) -L ./mlx -l mlx -framework OpenGL -Ofast -framework AppKit -L ./libft -l ft -lm -o $(NAME)
			@printf $(CUT) $(DOWN)
			@echo $(GREEN)miniRT compiled 🌏 $(RESET)

%.o : %.c
	@$(CC) -Wall -Wextra -Werror -Imlx -c $< -o $@
	@echo $(SAVE)$(CUT)$(GREEN) Compiling with $(CFLAGS)...$(RESET)
	@echo $(CUT) $(GREEN)[$^] to [$@] $(RESET)
	@printf $(UP)$(UP)

bonus : $(NAME)

clean:
	@rm -rf $(OBJ)
	@rm -f ./mlx/*.a
	@rm -f ./libft/*.o
	@rm -f ./libft/*.a
	@rm -f ./libft/gnl/*.o
	@rm -f ./libft/gnl/*.a

fclean: clean
	@rm -rf $(NAME)
	@echo $(GREEN)OBJ files removed 🧹$(RESET)

re: fclean all

.PHONY : all clean fclean re bonus
