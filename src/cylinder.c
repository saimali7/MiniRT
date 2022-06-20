#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

float	*vector_multiply(float	*vect, float value) // move to vector
{
	float *res;
	
	res = new_vect(vect[0] * value, vect[1] * value, vect[2] * value);
	return (res);
}

float	*vector_add(float *vect1, float *vect2)
{
	float	*res;

	res = new_vect(vect1[0] + vect2[0], vect1[1] + vect2[1], vect1[2] + vect2[2]);
	return (res);
}

int	solve_quadratic(float a, float b, float c, float *intersect)
{
	float	discriminant;
	float	q;

	discriminant = pow(b, 2) - 4 * a * c;
	if (discriminant < 0)
		return (0);
	else if (discriminant == 0)
	{
		intersect[0] = -0.5 * b / a;
		intersect[1] = -0.5 * b / a;
	}
	else
	{
		if (b > 0)
			q = -1 * (b + sqrt(discriminant)) / 2;
		else
			q = -1 * (b - sqrt(discriminant)) / 2;
		intersect[0] = q / a;
		intersect[1] = c / q;
	}
	if (intersect[0] > intersect[1])
	{
		q = intersect[0];
		intersect[0] = intersect[1];
		intersect[1] = q;
	}
	return (1);
}

int   cylinder_roots(t_rt *rt, float *direction, t_cylinder *cylinder, float *intersect)
{
    float	*a_sqrt;
	float	*right;
	float	a;
	float	b;
	float	c;
	float	*tmp;
	float	*tmp2;
	float	tmp3;

	tmp = vector_multiply(cylinder->orient, dot_product_vect(direction, cylinder->orient));
	a_sqrt = subtr_vec(direction, tmp);
	
	a = dot_product_vect(a_sqrt, a_sqrt);
	
	tmp2 = subtr_vec (rt->camera.origin, cylinder->coord); 
	tmp3 = 	dot_product_vect(subtr_vec(rt->camera.origin, cylinder->coord), cylinder->orient);
	right = subtr_vec(tmp2, vector_multiply(cylinder->orient, tmp3));
	b = 2 * dot_product_vect(a_sqrt, right);
	c = dot_product_vect(right, right) - ((cylinder->diametr / 2) * (cylinder->diametr / 2)); //change on radius
	
	if (!solve_quadratic(a, b, c, intersect))
	 	return (0);
	return (1);
}

void		check_t(float *inter, t_cylinder *cylinder, float *direction, t_rt *rt)
{
	float	*q;
	float	*p2;

	p2 = vector_add(cylinder->coord, vector_multiply(cylinder->orient, cylinder->height));
	q = vector_add(rt->camera.origin, vector_multiply(direction, *inter));
	if (dot_product_vect(cylinder->orient, subtr_vec(q, cylinder->coord)) <= 0)
		*inter = -1;
	if (dot_product_vect(cylinder->orient, subtr_vec(q, p2)) >= 0)
		*inter = -1;
}

int		intersect_cylinder(t_rt *rt, float *direction, t_cylinder *cylinder, float *intersect)	//, float *dist)
{
	if (!cylinder_roots(rt, direction, cylinder, intersect))
		return (0);
	if (intersect[0] > 0)
		check_t(&intersect[0], cylinder, direction, rt);
	if (intersect[1] > 0)
		check_t(&intersect[1], cylinder, direction, rt);
	if (intersect[0] < 0 && intersect[1] < 0)
		return (0);
	// if (intersect[1] < intersect[0])
	// 	if (intersect[1] > 0)
	// 		*dist = intersect[1];
	// 	else
	// 		*dist = intersect[0];
	// else
	// {
	// 	if (intersect[0] > 0)
	// 		*dist = intersect[0];
	// 	else
	// 		*dist = intersect[1];
	// }
	return (1);
}

int     trace_ray_cylinder(t_rt *rt, float *direction)
{
    t_cylinder *closest_cylinder = NULL;
    t_cylinder *cylinder;
    float *intersect;
    float closest_t = HUGE_VAL;
	//float dist;

    cylinder = rt->cylinder;
    intersect = malloc(sizeof(float) * 2);
    while (cylinder != NULL)
    {
		if (intersect_cylinder(rt, direction, cylinder, intersect))	//, &dist))
		{
			// if (dist < closest_t)
			// {
			// 	closest_cylinder = cylinder;
			// 	closest_t = dist;	
			// }
			if (intersect[0] < closest_t)
			{
				closest_cylinder = cylinder;
				closest_t = intersect[0];
			}
			if (intersect[1] < closest_t)
			{
				closest_cylinder = cylinder;
				closest_t = intersect[1];
			}
		}
        cylinder = cylinder->next;
    }
    if (closest_cylinder == NULL)
        return (0x000000);
    else
        return ((65536  * closest_cylinder->color[0]) + (256 * closest_cylinder->color[1]) + closest_cylinder->color[2]);
}