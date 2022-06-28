#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"


void	put_intersect(t_inter *intersect, float closest_t, char f)
{
	if (f == 'p')
	{
		intersect->closest_cylinder = NULL;
		intersect->closest_sphere = NULL;
	}
	else if (f == 's')
	{
		intersect->closest_cylinder = NULL;
		intersect->closest_plane = NULL;
	}
	else if (f == 'c')
	{
		intersect->closest_sphere = NULL;
		intersect->closest_plane = NULL;
	}
	intersect->closest_t = closest_t;
	intersect->hit_flag = 1;
}

float	intersect_plane(t_vect origin, t_vect dir, t_plane *plane, float *its)
{
	float	denom;
	t_vect	pl;

	denom = dot_product_vect(plane->orient, dir);
	if (fabs(denom) > 1e-6)
	{
		pl = subtr_vec(plane->coord, origin);
		*its = dot_product_vect(pl, plane->orient) / denom;
		return (*its >= 0);
	}
	return (0);
}

void	check_plane(t_vect origin, t_rt *rt, t_vect dir, t_inter *its)
{
	t_plane	*plane;
	t_plane	*closest_plane;
	float	intsect;
	float	closest_t;

	closest_t = INF;
	closest_plane = NULL;
	plane = rt->plane;
	while (plane != NULL)
	{
		intersect_plane(origin, dir, plane, &intsect);
		if (intsect < closest_t && its->min < intsect && intsect < its->max)
		{
			closest_t = intsect;
			closest_plane = plane;
		}
		plane = plane->next;
	}
	if (closest_plane != NULL && closest_t < its->closest_t)
	{
		its->closest_plane = closest_plane;
		put_intersect(its, closest_t, 'p');
	}
}