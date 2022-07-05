NAME = miniRT

 .SILENT:

CC = gcc

LIST =	src/main.c	src/geometry/plane.c	src/geometry/sphere.c	src/geometry/paraboloid.c	\
		src/geometry/vectors_p1.c	src/geometry/vectors_p2.c  	src/geometry/cylinder.c		src/parsing/parsing.c	\
		src/parsing/parse_sphere.c	src/parsing/parse_plane.c	src/parsing/parse_cylinder.c	src/parsing/parse_paraboloid.c	\
		src/parsing/parse_ambient_light.c 	src/parsing/parse_camera.c 	src/parsing/parsing_utils.c  src/parsing/parse_rt_line.c\
		src/render/calculate.c src/render/trace_ray.c	src/render/render_utils.c\
		src/utils/errors.c	src/utils/atod.c	src/utils/utils_free.c src/utils/extra_utils.c	\
		src/render/camera.c		src/render/chessboard.c	src/render/display.c	\
		src/render/render_shapes.c			src/render/render_shape_utils.c

LIST_BONUS = src_bonus/main_bonus.c	src_bonus/geometry_bonus/plane_bonus.c	src_bonus/geometry_bonus/sphere_bonus.c	src_bonus/geometry_bonus/paraboloid_bonus.c	\
		src_bonus/geometry_bonus/vectors_p1_bonus.c	src_bonus/geometry_bonus/vectors_p2_bonus.c  	src_bonus/geometry_bonus/cylinder_bonus.c		src_bonus/parsing_bonus/parsing_bonus.c	\
		src_bonus/parsing_bonus/parse_sphere_bonus.c	src_bonus/parsing_bonus/parse_plane_bonus.c	src_bonus/parsing_bonus/parse_cylinder_bonus.c	src_bonus/parsing_bonus/parse_paraboloid_bonus.c	\
		src_bonus/parsing_bonus/parse_ambient_light_bonus.c 	src_bonus/parsing_bonus/parse_camera_bonus.c 	src_bonus/parsing_bonus/parsing_utils_bonus.c  src_bonus/parsing_bonus/parse_rt_line_bonus.c\
		src_bonus/render_bonus/calculate_bonus.c src_bonus/render_bonus/trace_ray_bonus.c	src_bonus/render_bonus/render_utils_bonus.c\
		src_bonus/utils_bonus/errors_bonus.c	src_bonus/utils_bonus/atod_bonus.c	src_bonus/utils_bonus/utils_free_bonus.c src_bonus/utils_bonus/extra_utils_bonus.c	\
		src_bonus/render_bonus/camera_bonus.c		src_bonus/render_bonus/chessboard_bonus.c	src_bonus/render_bonus/display_bonus.c	\
		src_bonus/render_bonus/render_shapes_bonus.c			src_bonus/render_bonus/render_shape_utils_bonus.c

OBJS = $(LIST:.c=.o)

OBJS_BONUS = $(LIST_BONUS:.c=.o)

HEADERS = inc/MiniRt.h inc/Minilibx.h inc/Vector.h

CFLAGS = -Wall -Wextra -Werror

GREEN = "\033[32m"
RESET = "\033[0m"
CUT = "\033[K"
DOWN = "\033[B"
UP = "\033[A"
SAVE = "\033[s"

$(NAME) :	$(OBJS)
	$(MAKE) -C ./libft
	$(MAKE) -C ./mlx
	$(CC) $(CFLAGS) $(OBJS) -L ./mlx -l mlx -framework OpenGL -Ofast -framework AppKit -L ./libft -l ft -lm -o $(NAME)
	@printf $(CUT) $(DOWN)
	@echo $(GREEN)miniRT compiled 🌏 $(RESET)

all : $(NAME)

bonus : $(OBJS_BONUS)
	$(MAKE) -C ./libft
	$(MAKE) -C ./mlx
	$(CC) $(CFLAGS) $(OBJS_BONUS) -L ./mlx -l mlx -framework OpenGL -Ofast -framework AppKit -L ./libft -l ft -lm -o $(NAME)
	@printf $(CUT) $(DOWN)
	@echo $(GREEN)miniRT compiled 🌏 $(RESET)

clean:
	rm -rf $(OBJS)
	rm -rf ${OBJS_BONUS}
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
