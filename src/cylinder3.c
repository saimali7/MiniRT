#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

static double	cal_disc(t_vect origin, t_vect direction, t_cylinder *cylinder)
{
	float	a = dot_product_vect(direction, direction) - pow(dot_product_vect(direction, cylinder->orient), 2);
	t_vect	oc = subtr_vec(origin, cylinder->coord);
	float	half_b = dot_product_vect(direction, oc)
		- (dot_product_vect(oc, cylinder->orient) * dot_product_vect(direction, cylinder->orient));
	float	c = dot_product_vect(oc, oc) - pow(dot_product_vect(oc, cylinder->orient), 2) - (cylinder->radius * cylinder->radius);
	float	b = half_b * half_b;

	return (b - (a * c));
}

float	cal_root(t_vect origin, t_vect direction, t_cylinder *cylinder, int mode, float sqrtd)
{
	float	a = dot_product_vect(direction, direction)
		- pow(dot_product_vect(direction, cylinder->orient), 2);
	t_vect	oc = subtr_vec(origin, cylinder->coord);
	float	half_b = dot_product_vect(direction, oc) - (dot_product_vect(oc, cylinder->orient)
			* dot_product_vect(direction, cylinder->orient));
	if (mode == 1)
		return ((-half_b - sqrtd) / a);
	else
		return ((-half_b + sqrtd) / a);
}

int		intersect_cylinder(t_vect origin, t_vect direction, t_cylinder *cylinder, float *intsect, float *dist, t_inter *intersect)
{
    float	discriminant = cal_disc(origin, direction, cylinder);
    if (discriminant < 0)
		return (0);
    intsect[0] = cal_root(origin, direction, cylinder, 1, sqrt(discriminant));
    float   h0 = dot_product_vect(origin, cylinder->orient) + intsect[0] * dot_product_vect(direction, cylinder->orient)
		- dot_product_vect(cylinder->coord, cylinder->orient);
    if (h0 < 0 || h0 > cylinder->height || intsect[0] < intersect->min || intersect->max < intsect[0])
	{
		intsect[1] = cal_root(origin, direction, cylinder, 2, sqrt(discriminant));
		float h1 = dot_product_vect(origin, cylinder->orient) + intsect[1] * dot_product_vect(direction, cylinder->orient)
			- dot_product_vect(cylinder->coord, cylinder->orient);
		if (h1 < 0 || h1 > cylinder->height || intsect[1] < intersect->min || intersect->max < intsect[1])
			return (0);
	}
	if (intsect[1] < intsect[0])
	{
		if (intsect[1] > 0)
			*dist = intsect[1];
		else
			*dist = intsect[0];
	}
	else
	{
		if (intsect[0] > 0)
			*dist = intsect[0];
		else
			*dist = intsect[1];
	}
    return 1;
}

void	check_cylinder(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect)
{
    t_cylinder *closest_cylinder = NULL;
    t_cylinder *cylinder;
    float *intsect;
    float closest_t = INF;
	float dist;

    cylinder = rt->cylinder;
    intsect = malloc(sizeof(float) * 2);
    while (cylinder != NULL)
    {
		if (intersect_cylinder(origin, direction, cylinder, intsect, &dist, intersect))
		{
			if (dist < closest_t)
			{
				closest_cylinder = cylinder;
				closest_t = dist;
			}
		}
        cylinder = cylinder->next;
    }
	if (closest_cylinder == NULL)
    {
        free(intsect);
        return ;
    }
    intersect->closest_cylinder = closest_cylinder;
    intersect->closest_t = closest_t;
    intersect->closest_sphere = NULL;
    intersect->closest_plane = NULL;
    intersect->hit_flag = 1;
    free(intsect);
}