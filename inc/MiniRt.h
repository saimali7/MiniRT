#ifndef MINIRT_H
# define MINIRT_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "Minilibx.h"
#include "../mlx/mlx.h"
#include "../Libft/libft.h"
#include "../Libft/gnl/get_next_line.h"

typedef struct s_ambient
{
	float	ratio;
	int		color[3];
}			t_ambient;

typedef struct	s_camera
{
	float	coord[3];
	float	orient[3];
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
	float	coord[3];
	float	diametr;
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
	int		rgb[3];
	float	diametr;
	float	height;
	struct	s_cylinder *next;
}			t_cylinder;

typedef struct	s_rt
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_plane		*plane;
	t_sphere	*sphere;
	t_cylinder	*cylind;
	t_disp		*display;
}				t_rt;

double	rt_atod(const char *str, t_rt *rt);
void	init_window(t_disp *display);
void	free_rt(t_rt **ptr_rt);



#endif
