#include "../../inc/MiniRt.h"

int	solve_quadratic(float a, float b, float c, float *ip)
{
	float	discr;
	float	q;

	discr = pow(b, 2.0) - 4.0 * a * c;
	if (discr < 0)
		return (0);
	else if (discr == 0)
	{
		ip[0] = -0.5 * b / a;
		ip[1] = -0.5 * b / a;
	}
	else
	{
		if (b > 0.0)
			q = -1.0 * (b + sqrt(discr)) / 2.0;
		else
			q = -1.0 * (b - sqrt(discr)) / 2.0;
		ip[0] = q / a;
		ip[1] = c / q;
	}
	return (1);
}

void	intersect_parab(t_vect origin, t_vect dir, t_parab *parab, float *ip)
{
	float	a;
	float	b;
	float	c;
	t_vect	vecoc;

	vecoc = subtr_vec(origin, parab->extremum);
	a = dot_product(dir, dir) - powf(dot_product(dir, parab->orient), 2);
	b = 2 * (dot_product(vecoc, dir) - dot_product(dir, parab->orient) *  \
			(dot_product(vecoc, parab->orient) + 2 * parab->height));
	c = dot_product(vecoc, vecoc) - dot_product(vecoc, parab->orient) * \
			(dot_product(vecoc, parab->orient) + 4 * parab->height);
	solve_quadratic(a, b, c, ip);
}

void  check_paraboloid(t_vect origin, t_rt *rt, t_vect dir, t_inter *its)
{
    float closest_t = HUGE_VAL;
    t_parab *closest_parab = NULL;
    t_parab *parab;
    float *ip;

    parab = rt->parab;
    ip = malloc(sizeof(float) * 2);
    if (!ip)
        error_exit(-1, ERR_MEM_AL);;
    while (parab != NULL)
    {
        intersect_parab(origin, dir, parab, ip);
        if (ip[0] < closest_t && its->min < ip[0] && ip[0] < its->max)
        {
            closest_t = ip[0];
            closest_parab = parab;
        }
        if (ip[1] < closest_t && its->min < ip[0] && ip[0] < its->max)
        {
            closest_t = ip[1];
            closest_parab = parab;
        }
        parab = parab->next;
    }
    if (closest_parab != NULL && closest_t < its->closest_t)
	{
		its->closest_parab = closest_parab;
		put_intersect(its, closest_t, 'b');
	}
    free(ip);
}

