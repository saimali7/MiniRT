#include "../inc/MiniRt.h"
#include "../inc/Vector.h"

void	intersect_sphere(t_vect origin, t_vect direction, t_sphere *sphere, float *intersect)
{
	t_vect oc;
	float a;
	float b;
	float c;
	float discriminant;

	oc = subtr_vec(origin, sphere->coord);
	a = dot_product_vect(direction, direction);
	b = 2 * dot_product_vect(oc, direction);
	c = dot_product_vect(oc, oc) - sphere->radius * sphere->radius;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		intersect[0] = INF;
		intersect[1] = INF;
		return ;
	}
	intersect[0] = (-b + sqrt(discriminant)) / (2 * a);
	intersect[1] = (-b - sqrt(discriminant)) / (2 * a);
	return ;
}

void	check_sphere(t_vect origin, t_rt *rt, t_vect direction, t_inter *intersect)
{
	t_sphere	*sphere;
	t_sphere	*closest_sphere;
	float		*intsect;
	float		closest_t;

	closest_t = INF;
	closest_sphere = NULL;
	sphere = rt->sphere;
	intsect = calloc(sizeof(float), 2);
	while (sphere != NULL)
	{
		intersect_sphere(origin, direction, sphere, intsect);
		if (intsect[0] < closest_t &&intersect->min < intsect[0] && intsect[0] < intersect->max)
		{
			closest_t = intsect[0];
			closest_sphere = sphere;
		}
		if (intsect[1] < closest_t && intersect->min < intsect[1] && intsect[1] < intersect->max)
		{
			closest_t = intsect[1];
			closest_sphere = sphere;
		}
		sphere = sphere->next;
	}
	if (closest_sphere == NULL)
	{
		free(intsect);
		return ;
	}
	intersect->closest_sphere = closest_sphere;
	intersect->closest_t = closest_t;
	intersect->closest_cylinder = NULL;
	intersect->closest_plane = NULL;
	intersect->hit_flag = 1;
	free(intsect);
}