#include "../../inc/MiniRt.h"

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
	its.min = EPSILON;
	its.max = t_max;
	closest_intersection(point, rt, vec_light, &its);
	if (its.hit_flag != -1)
		return intensity;

    // diffuse light
	normal_dot = dot_product(normal, vec_light);
	if (normal_dot > 0)
		intensity += rt->light.ratio * normal_dot / (length_n * length_vect(vec_light));

    //specular light
	if (specular != -1)
	{
		r = subtr_vec(multiply_vect(2.0 * dot_product(normal, vec_light), normal), vec_light);
		r_dot_v = dot_product(r, view);
		if (r_dot_v > 0)
			intensity += rt->light.ratio * pow(r_dot_v / (length_vect(r) * length_v), specular);
	}
	return (intensity);
}

void	calculate(t_disp *display , t_rt *rt)
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
			color = trace_ray(rt, set_ray(rt->camera.coord, dir, 1, INF), 3);
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
