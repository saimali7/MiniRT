#include "../../inc/MiniRt.h"

void	intsect_sphere(t_vect origin, t_vect dir, t_sphere *sphere, float *ip)
{
	t_vect	oc;
	float	a;
	float	b;
	float	c;
	float	discr;

	oc = subtr_vec(origin, sphere->coord);
	a = dot_product(dir, dir);
	b = 2 * dot_product(oc, dir);
	c = dot_product(oc, oc) - sphere->radius * sphere->radius;
	discr = b * b - 4 * a * c;
	if (discr < 0)
	{
		ip[0] = INF;
		ip[1] = INF;
		return ;
	}
	ip[0] = (-b + sqrt(discr)) / (2 * a);
	ip[1] = (-b - sqrt(discr)) / (2 * a);
	return ;
}

void	check_sphere(t_vect origin, t_rt *rt, t_vect dir, t_inter *its)
{
	t_sphere	*sphere;
	t_sphere	*closest_sphere;
	float		*ip;
	float		closest_t;

	closest_t = INF;
	closest_sphere = NULL;
	sphere = rt->sphere;
	ip = ft_calloc(sizeof(float), 2);
	if (!ip)
		error_exit(-1, ERR_MEM_AL);
	while (sphere != NULL)
	{
		intsect_sphere(origin, dir, sphere, ip);
		if (ip[0] < closest_t && its->min < ip[0] && ip[0] < its->max)
		{
			closest_t = ip[0];
			closest_sphere = sphere;
		}
		if (ip[1] < closest_t && its->min < ip[1] && ip[1] < its->max)
		{
			closest_t = ip[1];
			closest_sphere = sphere;
		}
		sphere = sphere->next;
	}
	if (closest_sphere != NULL && closest_t < its->closest_t)
	{
		its->closest_sphere = closest_sphere;
		put_intersect(its, closest_t, 's');
	}
	free(ip);
}