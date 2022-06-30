#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

int	solve_quadratic(float a, float b, float c, float *intsect)
{
	float	discriminant;
	float	q;

	discriminant = pow(b, 2.0) - 4.0 * a * c;
	if (discriminant < 0)
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
	return (1);
}

void	intersect_parab(t_vect origin, t_vect dir, t_parab *parab, float *intersect)
{
	float	a;
	float	b;
	float	c;
	t_vect	vecoc;

	vecoc = subtr_vec(origin, parab->extremum);
	a = dot_product_vect(dir, dir) - powf(dot_product_vect(dir, parab->orient), 2);
	b = 2 * (dot_product_vect(vecoc, dir) - dot_product_vect(dir, parab->orient) *  (dot_product_vect(vecoc, parab->orient) + 2 * parab->height));
	c = dot_product_vect(vecoc, vecoc) - dot_product_vect(vecoc, parab->orient) * (dot_product_vect(vecoc, parab->orient) + 4 * parab->height);
	solve_quadratic(a, b, c, intersect);
}

void  check_paraboloid(t_vect origin, t_rt *rt, t_vect dir, t_inter *its)
{
    float closest_t = HUGE_VAL;
    t_parab *closest_parab = NULL;
    t_parab *parab;
    float *intsect;

    parab = rt->parab;
    intsect = malloc(sizeof(float) * 2);
    if (!intsect)
        error_exit(-1, ERR_MEM_AL);;
    while (parab != NULL)
    {
        intersect_parab(origin, dir, parab, intsect);
        if (intsect[0] < closest_t && its->min < intsect[0] && intsect[0] < its->max)
        {
            closest_t = intsect[0];
            closest_parab = parab;
        }
        if (intsect[1] < closest_t && its->min < intsect[0] && intsect[0] < its->max)
        {
            closest_t = intsect[1];
            closest_parab = parab;
        }
        parab = parab->next;
    }
    if (closest_parab != NULL && closest_t < its->closest_t)
	{
		its->closest_parab = closest_parab;
		put_intersect(its, closest_t, 'b');
	}
    free(intsect);
}

