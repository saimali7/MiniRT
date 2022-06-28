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
	else if(value.z < min)
		value.z = min;
	return (value);
}

void	ft_putpixel(int x, int y, t_vect color, t_disp *display)
{
	char	*temp;
	int		position;

	x = WIDHT/2 + x;
	y = HEIGHT/2 - y - 1;
	position = x * 4 + 4 * WIDHT * y;
	temp = display->img.addr;
	temp[position] = color.z;
	temp[position + 1] = color.y;
	temp[position + 2] = color.x;
}

t_vect	multiply_vectors(t_camera *cam, t_vect right, t_vect up, t_vect tmp)
{
	t_vect	res;

	res.x = tmp.x * right.x + tmp.y * up.x + tmp.z * cam->orient.x + cam->coord.x;
	res.y = tmp.x * right.y + tmp.y * up.y + tmp.z * cam->orient.y + cam->coord.y;
	res.z = tmp.x * right.z + tmp.y * up.z + tmp.z * cam->orient.z + cam->coord.z;
	return (res);
}

t_vect	get_direction(int x, int y, t_rt *rt)
{
	float	fov;
	float	aspect;

	fov = (float)rt->camera.fov;
	aspect = WIDHT / HEIGHT;
	float new_width = (tan(fov / 2 * (M_PI / 180))) * 2;
	float new_hight = new_width / aspect;
	float x_pix = new_width / WIDHT;
	float y_pix = new_hight / HEIGHT;
	return (new_vect(x * x_pix, y * y_pix, 1.0));
}

t_vect	get_look_right(t_camera *camera)
{
	t_vect	rand;
	t_vect	right;

	rand = new_vect(0, 1, 0);
	normalize_vect(&rand);
	right = cross_product(rand, camera->orient);
	normalize_vect(&right);
	return (right);
}

t_vect	get_look_up(t_camera *camera, t_vect right)
{
	t_vect	up;

	up = cross_product(camera->orient, right);
	normalize_vect(&up);
	//printf("x = %f, y = %f, z= %f;", up[0], up[1], up[2]); // ok
	return (up);
}

t_vect	convert_viewport(int x, int y, t_rt *rt)
{
	t_vect	direction;
	//t_vect	to_origin;
	t_vect	right;
	t_vect	up;

	right = get_look_right(&rt->camera);
	//printf("x = %f, y = %f, z= %f;", right[0], right[1], right[2]); //ok
	up = get_look_up(&rt->camera, right);
	rt->camera.origin = multiply_vectors(&rt->camera, right, up, new_vect(0, 0, 0));
	//printf("x = %f, y = %f, z= %f;", to_origin[0], to_origin[1], to_origin[2]); //ok
	// rt->camera.origin.x = to_origin.x;
	// rt->camera.origin.y = to_origin.y;
	// rt->camera.origin.z = to_origin.z;
	direction = get_direction(x, y, rt);
	direction = multiply_vectors(&rt->camera, right, up, direction);
	direction = subtr_vec(direction, rt->camera.origin);
	normalize_vect(&direction);
	return (direction);
}



void	check_plane(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect)
{
	t_plane	*plane;
	t_plane	*closest_plane;
	float	intsect;
	float	closest_t;

	closest_t = INF;
	closest_plane = NULL;
	plane = rt->plane;
	while (plane != NULL)
	{
		intersect_plane(origin, direction, plane, &intsect);
		if (intsect < closest_t && intersect->min < intsect && intsect < intersect->max)
		{
			closest_t = intsect;
			closest_plane = plane;
		}
		plane = plane->next;
	}
	if (closest_plane == NULL)
		return ;
	if (intersect->closest_t > closest_t)
	{
		intersect->closest_plane = closest_plane;
		intersect->closest_t = closest_t;
		intersect->closest_cylinder = NULL;
		intersect->closest_sphere = NULL;
		intersect->hit_flag = 1;
	}
}

void closest_intersection(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect)
{
	intersect->hit_flag = -1;
	intersect->closest_cylinder = NULL;
	intersect->closest_plane = NULL;
	intersect->closest_sphere = NULL;
	intersect->closest_t = INF;
	check_sphere(origin, rt, direction, intersect);
	check_plane(origin, rt, direction, intersect);
	check_cylinder(origin, rt, direction, intersect);
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
	t_inter	intersect;

	length_n = length_vect(normal);
	length_v = length_vect(view);
	intensity = 0.0;

	if (rt->ambient.ratio > 0)
	intensity += rt->ambient.ratio;

    //point light
	vec_light = subtr_vec(rt->light.coord, point);
	t_max = 1.0;

    // shadow check
	intersect.min = 0.0001;
	intersect.max = t_max;
	closest_intersection(point, rt, vec_light, &intersect);
	if (intersect.hit_flag != -1)
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

t_vect	trace_ray(t_rt *rt, t_vect direction, int min, int max)
{
	t_vect	point;
	t_vect	normal;
	t_vect	color;
	t_vect	view;
	t_inter	*intersect;

	intersect = malloc(sizeof(t_inter));
	intersect->closest_sphere = NULL;
	intersect->min = min;
	intersect->max = max;
	closest_intersection(rt->camera.origin, rt, direction, intersect);
	if (intersect->hit_flag == -1)
		return (new_vect(0,0,0));
	if (intersect->closest_sphere != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(intersect->closest_t, direction));
		normal = subtr_vec(point, intersect->closest_sphere->coord);
		normal = multiply_vect(1.0 / length_vect(normal), normal);
		view = multiply_vect(-1, direction);
		color = multiply_vect(lighting(point, normal, rt, 2000 , view), intersect->closest_sphere->color);
	}
	else if (intersect->closest_plane != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(intersect->closest_t, direction));
		normal = intersect->closest_plane->orient;
		normal = multiply_vect(1.0 / length_vect(normal), normal);
		view = multiply_vect(-1, direction);
		color = multiply_vect(lighting(point, normal, rt, 10, view), intersect->closest_plane->color);
	}
	else if (intersect->closest_cylinder != NULL)
	{
		point = add_vect(rt->camera.coord, multiply_vect(intersect->closest_t, direction));
        // normal = subtr_vec(point, intersect->closest_cylinder->coord);
        // normal = multiply_vect(1.0 / length_vect(normal), normal);
		normal = get_normal_for_cyl(point, intersect->closest_cylinder);
		view = multiply_vect(-1, direction);
		color = multiply_vect(lighting(point, normal, rt, 1500, view), intersect->closest_cylinder->color);
	}


	if (rt->ambient.ratio > 0.0)
	{
		color.x += (rt->ambient.color.x * rt->ambient.ratio);
		color.y += (rt->ambient.color.y * rt->ambient.ratio);
		color.z += (rt->ambient.color.z * rt->ambient.ratio);
	}
	free (intersect);
	return (color);
}

void	calculate_s(t_disp *display , t_rt *rt)
{
	int		x;
	int		y;
	t_vect	color;
	t_vect	direction;

	y = -HEIGHT/2;
	x = 0;

	while (y < HEIGHT/2)
	{
		x = -WIDHT/2;
		while (x < WIDHT/2)
		{
			direction = convert_viewport(x, y, rt);
			color = trace_ray(rt, direction, 1 , INF);
			ft_putpixel(x, y , clamp(0.0, 255.0, color), display);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0);
	mlx_hook(display->mlx_win, 17, 0, exit_hook, 0);
	mlx_key_hook(display->mlx_win, ft_key, rt);
	mlx_loop(display->mlx);
}
