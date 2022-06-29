#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

float	get_disc(t_vect origin, t_vect dir, t_cylinder *cylinder)
{
	float	a;
	float	b;
	float	c;
	float	half_b;
	t_vect	oc;
	
	a = dot_product_vect(dir, dir) - pow(dot_product_vect(dir, cylinder->orient), 2);
	oc = subtr_vec(origin, cylinder->coord);
	half_b = dot_product_vect(dir, oc) - (dot_product_vect(oc, cylinder->orient) * dot_product_vect(dir, cylinder->orient));
	c = dot_product_vect(oc, oc) - pow(dot_product_vect(oc, cylinder->orient), 2) - (cylinder->radius * cylinder->radius);
	b = half_b * half_b;
	return (b - (a * c));
}

float	get_roots(t_vect origin, t_vect dir, t_cylinder *cylinder, int mode, float sqrtd)
{
	float	a;
	float	half_b;
	t_vect	oc;
	
	a = dot_product_vect(dir, dir) - pow(dot_product_vect(dir, cylinder->orient), 2);
	oc = subtr_vec(origin, cylinder->coord);
	half_b = dot_product_vect(dir, oc) - (dot_product_vect(oc, cylinder->orient)
			* dot_product_vect(dir, cylinder->orient));
	if (mode == 1)
		return ((-half_b - sqrtd) / a);
	else
		return ((-half_b + sqrtd) / a);
}

int		intersect_cylinder(t_vect origin, t_vect dir, t_cylinder *cylinder, float *dist)
{
    float	tmp;
	float	h;
	float	discriminant;
	
	discriminant = get_disc(origin, dir, cylinder);
    if (discriminant < 0)
		return (0);
    tmp = get_roots(origin, dir, cylinder, 1, sqrt(discriminant));
    h = dot_product_vect(origin, cylinder->orient) + tmp * dot_product_vect(dir, cylinder->orient)
		- dot_product_vect(cylinder->coord, cylinder->orient);
    if (h < 0 || h > cylinder->height)
	{
		tmp = get_roots(origin, dir, cylinder, 2, sqrt(discriminant));
		h = dot_product_vect(origin, cylinder->orient) + tmp * dot_product_vect(dir, cylinder->orient)
			- dot_product_vect(cylinder->coord, cylinder->orient);
		if (h < 0 || h > cylinder->height)
			return (0);
	}
    *dist = tmp;
    return (1);
}

void	check_cylinder(t_vect origin, t_rt *rt, t_vect dir, t_inter *its)
{
    t_cylinder *closest_cylinder;
    t_cylinder *cylinder;
    float closest_t;
	float dist;

	closest_t = INF;
    cylinder = rt->cylinder;
	closest_cylinder = NULL;
    while (cylinder != NULL)
    {
		if (intersect_cylinder(origin, dir, cylinder, &dist))
		{
			if (dist < closest_t && its->min < dist && dist < its->max)
			{
				closest_cylinder = cylinder;
				closest_t = dist;
			}
		}
        cylinder = cylinder->next;
    }
	if (closest_cylinder != NULL && closest_t < its->closest_t)
    {
		its->closest_cylinder = closest_cylinder;
		put_intersect(its, closest_t, 'c');
	}
}