#include "../../inc/MiniRt.h"

t_vect		get_normal_for_cyl(t_vect point, t_cylndr *cylndr)
{
	t_vect tmp;
	t_vect normal;

	tmp = subtr_vec(point, cylndr->coord);
	normal = subtr_vec(tmp, multiply_vect(dot_product(cylndr->orient, tmp), cylndr->orient));
	normalize_vect(&normal);
	return (normal);
}

t_vect		get_normal_for_parab(t_vect point, t_parab *parab)
{
	t_vect tmp;
	t_vect normal;

	tmp = subtr_vec(point, parab->extremum);
	normal = subtr_vec(tmp, multiply_vect(dot_product(parab->orient, tmp), parab->orient));
	normalize_vect(&normal);
	return (normal);
}

t_vect	trace_ray(t_rt *rt, t_ray ray, int recurse)
{
	t_vect	point;
	t_vect	normal;
	t_vect	color;
	t_vect	view;
	t_inter	its;
	t_vect r_ray;
	t_vect reflect_color;

	color.x = 0;
	color.y = 0;
	color.z = 0;
	its.closest_sphere = NULL;
	its.min = ray.min;
	its.max = ray.max;
	closest_intersection(ray.origin, rt, ray.dir, &its);
	if (its.hit_flag == -1)
		return (new_vect(0,0,0));
	if (its.closest_sphere != NULL)
	{
		point = add_vect(ray.origin, multiply_vect(its.closest_t, ray.dir));
		normal = subtr_vec(point, its.closest_sphere->coord);
		normal = multiply_vect(1.0 / length_vect(normal), normal);
		view = multiply_vect(-1, ray.dir);
		if (its.closest_sphere->is_chess)
		{
			t_vect even_color = its.closest_sphere->color;
			t_vect odd_color = inverse_color(even_color);
			color = chessboard(ray, even_color, odd_color, its.closest_t, its.closest_sphere->is_chess);
			color = multiply_vect(lighting(point, normal, rt, its.closest_sphere->specular, view), color);
		}
		else
			color = multiply_vect(lighting(point, normal, rt, its.closest_sphere->specular, view), its.closest_sphere->color);
		if (its.closest_sphere->reflect > 0 && recurse > 0)
		{
			r_ray = subtr_vec(multiply_vect(2.0 * dot_product(view, normal), normal), view);
			reflect_color = trace_ray(rt, set_ray(point, r_ray, EPSILON, INF), recurse - 1);
			color = add_vect(multiply_vect(1.0 - its.closest_sphere->reflect, color), multiply_vect(its.closest_sphere->reflect, reflect_color));
		}
	}
	else if (its.closest_plane != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(its.closest_t, ray.dir));
		normal = its.closest_plane->orient;
		normal = multiply_vect(1.0 / length_vect(normal), normal);
		view = multiply_vect(-1, ray.dir);
		if (its.closest_plane->is_chess)
		{
			t_vect even_color = its.closest_plane->color;
			t_vect odd_color = inverse_color(even_color);
			color = chessboard(ray, even_color, odd_color, its.closest_t, its.closest_plane->is_chess);
			color = multiply_vect(lighting(point, normal, rt, its.closest_plane->specular, view), color);
		}
		else
			color = multiply_vect(lighting(point, normal, rt, its.closest_plane->specular, view), its.closest_plane->color);
		if (its.closest_plane->reflect > 0 && recurse > 0)
		{
			r_ray = subtr_vec(multiply_vect(2.0 * dot_product(view, normal), normal), view);
			reflect_color = trace_ray(rt, set_ray(point, r_ray, EPSILON, INF), recurse - 1);
			color = add_vect(multiply_vect(1.0 - its.closest_plane->reflect, color), multiply_vect(its.closest_plane->reflect, reflect_color));
		}
	}
	else if (its.closest_cylndr != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(its.closest_t, ray.dir));
        // normal = subtr_vec(point, its.closest_cylinder->coord);
        // normal = multiply_vect(1.0 / length_vect(normal), normal);
		normal = get_normal_for_cyl(point, its.closest_cylndr);
		view = multiply_vect(-1, ray.dir);
		if (its.closest_cylndr->is_chess)
		{
			t_vect even_color = its.closest_cylndr->color;
			t_vect odd_color = inverse_color(even_color);
			color = chessboard(ray, even_color, odd_color, its.closest_t, its.closest_cylndr->is_chess);
			color = multiply_vect(lighting(point, normal, rt, its.closest_cylndr->specular, view), color);
		}
		else
			color = multiply_vect(lighting(point, normal, rt, its.closest_cylndr->specular, view), its.closest_cylndr->color);
		if (its.closest_cylndr->reflect > 0 && recurse > 0)
		{
			r_ray = subtr_vec(multiply_vect(2.0 * dot_product(view, normal), normal), view);
			reflect_color = trace_ray(rt, set_ray(point, r_ray, EPSILON, INF), recurse - 1);
			color = add_vect(multiply_vect(1.0 - its.closest_cylndr->reflect, color), multiply_vect(its.closest_cylndr->reflect, reflect_color));
		}
	}
	else if (its.closest_parab != NULL)
	{
		point = add_vect(ray.origin, multiply_vect(its.closest_t, ray.dir));
		// normal = subtr_vec(point, its.closest_parab->extremum);
		// normal = multiply_vect(1.0 / length_vect(normal), normal);
		normal = get_normal_for_parab(point, its.closest_parab);
		view = multiply_vect(-1, ray.dir);
		if (its.closest_parab->is_chess)
		{
			t_vect even_color = its.closest_parab->color;
			t_vect odd_color = inverse_color(even_color);
			color = chessboard(ray, even_color, odd_color, its.closest_t, its.closest_parab->is_chess);
			color = multiply_vect(lighting(point, normal, rt, its.closest_parab->specular, view), color);
		}
		else
		color = multiply_vect(lighting(point, normal, rt, its.closest_parab->specular, view), its.closest_parab->color);
		if (its.closest_parab->reflect > 0 && recurse > 0)
		{
			r_ray = subtr_vec(multiply_vect(2.0 * dot_product(view, normal), normal), view);
			reflect_color = trace_ray(rt, set_ray(point, r_ray, EPSILON, INF), recurse - 1);
			color = add_vect(multiply_vect(1.0 - its.closest_parab->reflect, color), multiply_vect(its.closest_parab->reflect, reflect_color));
		}
	}
	if (rt->ambient.ratio > 0.0)
	{
		color.x += (rt->ambient.color.x * rt->ambient.ratio) * color.x / 255;
		color.y += (rt->ambient.color.y * rt->ambient.ratio) * color.y / 255;
		color.z += (rt->ambient.color.z * rt->ambient.ratio) * color.z / 255;
	}
	return (color);
}