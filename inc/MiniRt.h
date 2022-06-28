#ifndef MINIRT_H
# define MINIRT_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "Minilibx.h"
#include "Vector.h"
#include "../mlx/mlx.h"
#include "../Libft/libft.h"
#include "../Libft/gnl/get_next_line.h"

# define MAX_SIZE 10000
# define RAD
# define ERR_MEM_AL "Error: allocating memory\n"

typedef struct s_ambient
{
	float	ratio;
	t_vect	color;
}			t_ambient;

typedef struct	s_camera
{
	t_vect	coord;
	t_vect	orient;
	int		fov;
	t_vect	origin; // need to add in parse NULL
}			t_camera;

typedef struct	s_light
{
	t_vect	coord;
	float	ratio;
	t_vect	color;
}			t_light;

typedef struct s_sphere
{
	t_vect	coord;
	float	diametr;
	float	radius;
	t_vect	color;
	struct	s_sphere	*next;
}			t_sphere;

typedef struct	s_plane
{
	t_vect	coord;
	t_vect	orient;
	t_vect	color;
	struct s_plane	*next;
}			t_plane;

typedef struct s_cylinder
{
	t_vect	coord;
	t_vect	orient;
	t_vect	color;
	float	radius;
	float	height;
	struct	s_cylinder *next;
}			t_cylinder;

typedef struct s_intersect
{
	t_plane		*closest_plane;
	t_sphere	*closest_sphere;
	t_cylinder	*closest_cylinder;
	float		 closest_t;
	float		min;
	float		max;
	int			hit_flag;
}				t_inter;

typedef struct	s_rt
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_plane		*plane;
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	t_disp		*display;
}				t_rt;

int		parse(t_rt *rt, char *arg);
double	rt_atod(const char *str, t_rt *rt);
void	init_window(t_disp *display, t_rt *rt);
void	ft_calculate(t_disp *display, t_rt *rt);
void	free_rt(t_rt **ptr_rt);
void	error_exit(int code, char *str);
void	ray_tracing(t_rt *rt);
float	is_ray_sphere(t_camera camera, float *ray, t_sphere *sphere);
unsigned int	ft_get_rgb(int *color);
void	calculate_s(t_disp *display , t_rt *rt);
void	free_all(t_rt *ptr_rt);
int		exit_hook(int x);
int		ft_key(int key, t_rt *rt);

int		trace_ray_plane(t_rt *rt, float *direction);
void	check_sphere(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect);
void	check_cylinder(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect);
float	intersect_plane(t_vect origin, t_vect direction, t_plane *plane, float *intersect);

#endif
