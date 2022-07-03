#include "../../inc/MiniRt.h"

void	init_light_var(t_lightvar *var, t_rt *rt, t_vect point)
{
	var->intensity = 0.0;
	if (rt->ambient.ratio > 0)
		var->intensity += rt->ambient.ratio;
	var->vec_light = subtr_vec(rt->light.coord, point);
	var->its.min = EPSILON;
	var->its.max = 1.0;
}

float	lighting(t_vect point, t_vect normal, t_rt *rt, float specular, t_vect view)
{
	t_lightvar var;

	var.length_n = length_vect(normal);
	var.length_v = length_vect(view);
	init_light_var(&var, rt, point);
	closest_intersection(point, rt, var.vec_light, &var.its);
	if (var.its.hit_flag != -1)
		return var.intensity;
	var.normal_dot = dot_product(normal, var.vec_light);
	if (var.normal_dot > 0)
		var.intensity += rt->light.ratio * var.normal_dot
		/ (var.length_n * length_vect(var.vec_light));
	if (specular != -1)
	{
		var.r = subtr_vec(multiply_vect(2.0 * dot_product(normal,
		var.vec_light), normal), var.vec_light);
		var.r_dot_v = dot_product(var.r, view);
		if (var.r_dot_v > 0)
			var.intensity += rt->light.ratio * pow(var.r_dot_v
			/ (length_vect(var.r) * var.length_v), specular);
	}
	return (var.intensity);
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
