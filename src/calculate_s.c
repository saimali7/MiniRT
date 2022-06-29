#include "../inc/MiniRt.h"
#include "../inc/Vector.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

t_vect	clamp(float min, float max, t_vect value)
{
	if (value.x > max)
		value.x = max;
	else if (value.x < min)
		value.x = min;
	if (value.y > max)
		value.y = max;
	else if (value.y < min)
		value.y = min;
	if (value.z > max)
		value.z = max;
	else if (value.z < min)
		value.z = min;
	return (value);
}

void	ft_putpixel(int x, int y, t_vect color, t_disp *display)
{
	char	*temp;
	int		position;

	x = WIDHT /2 + x;
	y = HEIGHT /2 - y - 1;
	position = x * 4 + 4 * WIDHT * y;
	temp = display->img.addr;
	temp[position] = color.z;
	temp[position + 1] = color.y;
	temp[position + 2] = color.x;
}

void	closest_intersection(t_vect origin, t_rt *rt, t_vect dir, t_inter *its)
{
	its->hit_flag = -1;
	its->closest_cylinder = NULL;
	its->closest_plane = NULL;
	its->closest_sphere = NULL;
	its->closest_t = INF;
	check_sphere(origin, rt, dir, its);
	check_cylinder(origin, rt, dir, its);
	check_plane(origin, rt, dir, its);
}

float	lighting(t_vect point, t_vect normal, t_rt *rt, float specular, t_vect view)
{
	float	intensity;
	t_vect	vec_light;
	float	normal_dot;
	float	length_n;
	float	length_v;
	t_vect	r;
	float	r_dot_v;
	float	t_max;
	t_inter	its;

	length_n = length_vect(normal);
	length_v = length_vect(view);
	intensity = 0.0;

	if (rt->ambient.ratio > 0)
	intensity += rt->ambient.ratio;

    //point light
	vec_light = subtr_vec(rt->light.coord, point);
	t_max = 1.0;

    // shadow check
	its.min = 0.0001;
	its.max = t_max;
	closest_intersection(point, rt, vec_light, &its);
	if (its.hit_flag != -1)
		return intensity;

    // diffuse light
	normal_dot = dot_product_vect(normal, vec_light);
	if (normal_dot > 0)
		intensity += rt->light.ratio * normal_dot / (length_n * length_vect(vec_light));

    //specular light
	if (specular != -1)
	{
		r = subtr_vec(multiply_vect(2.0 * dot_product_vect(normal, vec_light), normal), vec_light);
		r_dot_v = dot_product_vect(r, view);
		if (r_dot_v > 0)
			intensity += rt->light.ratio * pow(r_dot_v / (length_vect(r) * length_v), specular);
	}
	return (intensity);
}

t_vect		get_normal_for_cyl(t_vect point, t_cylinder *cylinder)
{
	t_vect tmp;
	t_vect normal;

	tmp = subtr_vec(point, cylinder->coord);
	normal = subtr_vec(tmp, multiply_vect(dot_product_vect(cylinder->orient, tmp), cylinder->orient));
	normalize_vect(&normal);
	return (normal);
}

t_vect	trace_ray(t_rt *rt, t_vect dir, int min, int max)
{
	t_vect	point;
	t_vect	normal;
	t_vect	color;
	t_vect	view;
	t_inter	*its;

	its = malloc(sizeof(t_inter));
	if (!its)
		error_exit(-1, ERR_MEM_AL);;
	its->closest_sphere = NULL;
	its->min = min;
	its->max = max;
	closest_intersection(rt->camera.origin, rt, dir, its);
	if (its->hit_flag == -1)
		return (new_vect(0,0,0));
	if (its->closest_sphere != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(its->closest_t, dir));
		normal = subtr_vec(point, its->closest_sphere->coord);
		normal = multiply_vect(1.0 / length_vect(normal), normal);
		view = multiply_vect(-1, dir);
		color = multiply_vect(lighting(point, normal, rt, 2000 , view), its->closest_sphere->color);
	}
	else if (its->closest_plane != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(its->closest_t, dir));
		normal = its->closest_plane->orient;
		normal = multiply_vect(1.0 / length_vect(normal), normal);
		view = multiply_vect(-1, dir);
		color = multiply_vect(lighting(point, normal, rt, 10, view), its->closest_plane->color);
	}
	else if (its->closest_cylinder != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(its->closest_t, dir));
        //normal = subtr_vec(point, its->closest_cylinder->coord);
        //normal = multiply_vect(1.0 / length_vect(normal), normal);
		normal = get_normal_for_cyl(point, its->closest_cylinder);
		view = multiply_vect(-1, dir);
		color = multiply_vect(lighting(point, normal, rt, 1500, view), its->closest_cylinder->color);
	}

	if (rt->ambient.ratio > 0.0)
	{
		color.x += (rt->ambient.color.x * rt->ambient.ratio);
		color.y += (rt->ambient.color.y * rt->ambient.ratio);
		color.z += (rt->ambient.color.z * rt->ambient.ratio);
	}
	free (its);
	return (color);
}

void	calculate_s(t_disp *display , t_rt *rt)
{
	int		x;
	int		y;
	t_vect	color;
	t_vect	dir;

	y = -HEIGHT / 2;
	x = 0;
	while (y < HEIGHT / 2)
	{
		x = -WIDHT / 2;
		while (x < WIDHT / 2)
		{
			dir = convert_viewport(x, y, rt);
			color = trace_ray(rt, dir, 1, INF);
			ft_putpixel(x, y, clamp(0.0, 255.0, color), display);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0);
	mlx_hook(display->mlx_win, 17, 0, exit_hook, 0);
	mlx_key_hook(display->mlx_win, ft_key, rt);
	mlx_loop(display->mlx);
}
