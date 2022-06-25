#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

int	solve_quadratic(float a, float b, float c, float *intsect)
{
	float	discriminant;
	float	q;

	discriminant = pow(b, 2) - 4 * a * c;
	if (discriminant < 0)
		return (0);
	else if (discriminant == 0)
	{
		intsect[0] = -0.5 * b / a;
		intsect[1] = -0.5 * b / a;
	}
	else
	{
		if (b > 0)
			q = -1 * (b + sqrt(discriminant)) / 2;
		else
			q = -1 * (b - sqrt(discriminant)) / 2;
		intsect[0] = q / a;
		intsect[1] = c / q;
	}
	if (intsect[0] > intsect[1])
	{
		q = intsect[0];
		intsect[0] = intsect[1];
		intsect[1] = q;
	}
	return (1);
}

int   cylinder_roots(t_vect origin, t_vect direction, t_cylinder *cylinder, float *intsect)
{
    t_vect	a_sqrt;
	t_vect	right;
	//float	coeff[3];
	float	a;
	float	b;
	float	c;
	t_vect	tmp;
	t_vect	tmp2;
	float	tmp3;

	tmp = multiply_vect(dot_product_vect(direction, cylinder->orient), cylinder->orient);
	a_sqrt = subtr_vec(direction, tmp);
	
	a = dot_product_vect(a_sqrt, a_sqrt);
	
	tmp2 = subtr_vec (origin, cylinder->coord); 
	tmp3 = dot_product_vect(subtr_vec(origin, cylinder->coord), cylinder->orient);
	right = subtr_vec(tmp2, multiply_vect(tmp3, cylinder->orient));
	b = 2 * dot_product_vect(a_sqrt, right);
	c = dot_product_vect(right, right) - ((cylinder->radius) * (cylinder->radius)); //change on radius

	if (!solve_quadratic(a, b, c, intsect))
	 	return (0);
	return (1);
}

void		check_intersect(float *intsect, t_cylinder *cylinder, t_vect direction, t_vect origin)
{
	t_vect	q;
	t_vect	p2;

	p2 = add_vect(cylinder->coord, multiply_vect(cylinder->height, cylinder->orient));
	q = add_vect(origin, multiply_vect(*intsect, direction));
	if (dot_product_vect(cylinder->orient, subtr_vec(q, cylinder->coord)) <= 0)
		*intsect = -1;
	if (dot_product_vect(cylinder->orient, subtr_vec(q, p2)) >= 0)
		*intsect = -1;
}

int		intersect_cylinder(t_vect origin, t_vect direction, t_cylinder *cylinder, float *intsect)
{
	if (!cylinder_roots(origin, direction, cylinder, intsect))
		return (0);
	if (intsect[0] > 0)
		check_intersect(&intsect[0], cylinder, direction, origin);
	if (intsect[1] > 0)
		check_intersect(&intsect[1], cylinder, direction, origin);
	if (intsect[0] < 0 && intsect[1] < 0)
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

void    trace_ray_cylinder(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect)
{
    t_cylinder *closest_cylinder = NULL;
    t_cylinder *cylinder;
    float *intsect;
    float closest_t = HUGE_VAL;
	//float dist;

    cylinder = rt->cylinder;
    intsect = malloc(sizeof(float) * 2);
    while (cylinder != NULL)
    {
		if (intersect_cylinder(origin, direction, cylinder, intsect))
		{
			// if (dist < closest_t)
			// {
			// 	closest_cylinder = cylinder;
			// 	closest_t = dist;	
			// }
			if (intsect[0] < closest_t)
			{
				closest_cylinder = cylinder;
				closest_t = intsect[0];
			}
			if (intsect[1] < closest_t)
			{
				closest_cylinder = cylinder;
				closest_t = intsect[1];
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