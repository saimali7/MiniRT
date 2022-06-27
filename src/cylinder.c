#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"


// double	cylinder_discriminant(t_ray *ray, t_cy *cy, t_cylinder *ptr)
// {
// 	double	discriminant;

// 	cy->oc = vector_subtraction(ray->origin, ptr->coordinates);
// 	cy->a = pow(ray->direction[0], 2) + pow(ray->direction[2], 2);
// 	cy->b = 2.0 * cy->oc[0] * ray->direction[0] + 2.0 * cy->oc[2] \
// 	* ray->direction[2];
// 	cy->c = pow(cy->oc[0], 2) + pow(cy->oc[2], 2) - (ptr->diameter / 2);
// 	discriminant = (pow(cy->b, 2)) - (4 * cy->a * cy->c);
// 	cy->t0 = (-cy->b - sqrt(discriminant)) / (2.0 * cy->a);
// 	cy->t1 = (-cy->b + sqrt(discriminant)) / (2.0 * cy->a);
// 	cy->max = (pow((ptr->diameter / 2), 2)) / 2.0;
// 	cy->min = -1.0 * cy->max;
// 	return (discriminant);
// }



int	solve_quadratic(float a, float b, float c, float *intsect)
{
	float	discriminant;
	float	q;

	discriminant = pow(b, 2.0) - 4.0 * a * c;
	if (discriminant < 0.0)
		return (0);
	else if (discriminant == 0)
	{
		intsect[0] = -0.5 * b / a;
		intsect[1] = -0.5 * b / a;
	}
	else
	{
		if (b > 0.0)
			q = -1.0 * (b + sqrt(discriminant)) / 2.0;
		else
			q = -1.0 * (b - sqrt(discriminant)) / 2.0;
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

int	cylinder_roots(t_vect origin, t_vect direction, t_cylinder *cylinder, float *intsect)
{
    // t_vect	a_sqrt;
	// t_vect	right;
	// //float	coeff[3];
	// float	a;
	// float	b;
	// float	c;
	// t_vect	tmp;
	// t_vect	tmp2;
	// float	tmp3;

	// tmp = multiply_vect(dot_product_vect(direction, cylinder->orient), cylinder->orient);
	// a_sqrt = subtr_vec(direction, tmp);

	// a = dot_product_vect(a_sqrt, a_sqrt);

	// tmp2 = subtr_vec (origin, cylinder->coord);
	// tmp3 = dot_product_vect(subtr_vec(origin, cylinder->coord), cylinder->orient);
	// right = subtr_vec(tmp2, multiply_vect(tmp3, cylinder->orient));
	// b = 2 * dot_product_vect(a_sqrt, right);
	// c = dot_product_vect(right, right) - ((cylinder->radius) * (cylinder->radius));

	t_vect	a_sqrt;
	t_vect	right;
	float	a;
	float	b;
	float	c;

	a_sqrt = subtr_vec(direction, multiply_vect(dot_product_vect(direction, cylinder->orient), cylinder->orient));
	a = dot_product_vect(a_sqrt, a_sqrt);
	right = subtr_vec(subtr_vec(origin, cylinder->coord), multiply_vect(dot_product_vect(subtr_vec(origin, cylinder->coord), cylinder->orient), cylinder->orient));
	b = 2.0 * dot_product_vect(a_sqrt, right);
	c = dot_product_vect(right, right) - (cylinder->radius * cylinder->radius);

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
		*intsect = -1.0;
	if (dot_product_vect(cylinder->orient, subtr_vec(q, p2)) >= 0)
		*intsect = -1.0;
}

int		intersect_cylinder(t_vect origin, t_vect direction, t_cylinder *cylinder, float *intsect, float *dist)
{
	if (!cylinder_roots(origin, direction, cylinder, intsect))
		return (0);
	if (intsect[0] > 0)
		check_intersect(&intsect[0], cylinder, direction, origin);
	if (intsect[1] > 0)
		check_intersect(&intsect[1], cylinder, direction, origin);
	if (intsect[0] < 0 && intsect[1] < 0)
		return (0);
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
	return (1);
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
		if (intersect_cylinder(origin, direction, cylinder, intsect, &dist))
		{
			if (dist < closest_t)
			{
				closest_cylinder = cylinder;
				closest_t = dist;
			}
			// if (intsect[0] < closest_t)
			// {
			// 	closest_cylinder = cylinder;
			// 	closest_t = intsect[0];
			// }
			// if (intsect[1] < closest_t)
			// {
			// 	closest_cylinder = cylinder;
			// 	closest_t = intsect[1];
			// }
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