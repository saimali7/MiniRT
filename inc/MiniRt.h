#ifndef MINIRT_H
# define MINIRT_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "Minilibx.h"
#include "../mlx/mlx.h"
#include "Vectors.h"
#include "../Libft/libft.h"
#include "../Libft/gnl/get_next_line.h"

# define MAX_SIZE 10000
# define RAD
# define ERR_MEM_AL "Error: allocating memory\n"

typedef struct	s_vect	t_vect;

typedef struct s_ambient
{
	float	ratio;
	int		color[3];
}			t_ambient;

typedef struct	s_camera
{
	float	coord[3];	//vector
	float	orient[3];	//vector
	int		fov;
}	t_camera;

typedef struct	s_light
{
	float	coord[3];
	float	ratio;
	int		color[3];
}			t_light;

typedef struct s_sphere
{
	float	coord[3]; // change t_vect	*center;
	float	diametr;
	float	radius;
	int		color[3];
	struct s_sphere	*next;
}			t_sphere;

typedef struct	s_plane
{
	float	coord[3];
	float	orient[3];
	int		color[3];
	struct s_plane	*next;
}			t_plane;

typedef struct s_cylinder
{
	float	coord[3];
	float	orient[3];
	int		color[3];
	float	diametr;
	float	height;
	struct	s_cylinder *next;
}			t_cylinder;

typedef struct	s_rt
{
	// void		*mlx;
	// void		*mlx_win;
	// t_img		img;
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_plane		*plane;
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	t_disp		*display;
}				t_rt;

double	rt_atod(const char *str, t_rt *rt);
void	init_window(t_disp *display, t_rt *rt);
void	ft_calculate(t_disp *display, t_rt *rt);
void	free_rt(t_rt **ptr_rt);
void	error_exit(int code, char *str);
void    ray_tracing(t_rt *rt);
float	is_ray_sphere(t_camera camera, float *ray, t_sphere *sphere);
unsigned int	ft_get_rgb(int *color);



#endif
