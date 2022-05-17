#include "../inc/Vectors.h"

t_vect	*new_vect(float x, float y, float z)
{
	t_vect	*new;

	new = malloc(sizeof(t_vect));
	if (!new)
		error_exit(-1, ERR_MEM_AL);
	new->x = x;
    new->y = y;
 	new->z = z;
 	return (new);
}

t_vect	*subtr_vec(t_vect *vect1, t_vect *vect2)
{
	t_vect	*res;

 	res = new_vect(vect1->x - vect2->x, vect1->y - vect2->y, vect1->z - vect2->z);
	return (res);
}

float	lenght_vect(t_vect *vect)
{
	float	res;

	res = sqrt((vect->x * vect->x) + (vect->y * vect->y) + (vect->z * vect->z));
	return (res);
}

void	normalize_vect(t_vect *vect)
{
	float	length;

	length = lenght_vect(vect);
	vect->x /= length;
	vect->y /= length;
	vect->z /= length;
}

float	dot_product_vect(t_vect *vect1, t_vect *vect2)
{
	float	res;

	res = ((vect1->x * vect2->x) + (vect1->y * vect2->y) + (vect1->z * vect2->z));
	return (res);
}

