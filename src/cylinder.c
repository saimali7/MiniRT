#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

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
	//float	coeff[3];
	float	a;
	float	b;
	float	c;
	float	*tmp;
	float	*tmp2;
	float	tmp3;

	tmp = multiply_vect(dot_product_vect(direction, cylinder->orient), cylinder->orient);
	a_sqrt = subtr_vec(direction, tmp);
	
	a = dot_product_vect(a_sqrt, a_sqrt);
	
	tmp2 = subtr_vec (rt->camera.origin, cylinder->coord); 
	tmp3 = 	dot_product_vect(subtr_vec(rt->camera.origin, cylinder->coord), cylinder->orient);
	right = subtr_vec(tmp2, multiply_vect(tmp3, cylinder->orient));
	b = 2 * dot_product_vect(a_sqrt, right);
	c = dot_product_vect(right, right) - ((cylinder->diametr / 2) * (cylinder->diametr / 2)); //change on radius
	
	free(tmp);
	free(tmp2);
	if (!solve_quadratic(a, b, c, intersect))
	 	return (0);
	return (1);
}

void		check_intersect(float *inter, t_cylinder *cylinder, float *direction, t_rt *rt)
{
	float	*q;
	float	*p2;

	p2 = add_vect(cylinder->coord, multiply_vect(cylinder->height, cylinder->orient));
	q = add_vect(rt->camera.origin, multiply_vect(*inter, direction));
	if (dot_product_vect(cylinder->orient, subtr_vec(q, cylinder->coord)) <= 0)
		*inter = -1;
	if (dot_product_vect(cylinder->orient, subtr_vec(q, p2)) >= 0)
		*inter = -1;
	free(q);
	free(p2);
}

int		intersect_cylinder(t_rt *rt, float *direction, t_cylinder *cylinder, float *intersect)	//, float *dist)
{
	if (!cylinder_roots(rt, direction, cylinder, intersect))
		return (0);
	if (intersect[0] > 0)
		check_intersect(&intersect[0], cylinder, direction, rt);
	if (intersect[1] > 0)
		check_intersect(&intersect[1], cylinder, direction, rt);
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
	free(intersect);
    if (closest_cylinder == NULL)
        return (0x000000);
    else
        return ((65536  * closest_cylinder->color[0]) + (256 * closest_cylinder->color[1]) + closest_cylinder->color[2]);
}