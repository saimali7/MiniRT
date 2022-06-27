#include "../inc/Vector.h"

t_vect	new_vect(float x, float y, float z)
{
	t_vect result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

t_vect	subtr_vec(t_vect v1, t_vect v2)
{
	t_vect	result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return (result);
}

float	length_vect(t_vect res)
{
	float	temp;

	temp = sqrt((res.x * res.x) + (res.y * res.y) + (res.z * res.z));
	return (temp);
}

void	normalize_vect(t_vect *vect)
{
	float	length;

	length = length_vect(*vect);
	vect->x /= length;
	vect->y /= length;
	vect->z /= length;
}

float	dot_product_vect(t_vect vect1, t_vect vect2)
{
	float	res;

	res = ((vect1.x * vect2.x) + (vect1.y * vect2.y) + (vect1.z * vect2.z));
	return (res);
}

t_vect	cross_product(t_vect v1, t_vect v2)
{
	t_vect	result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}

t_vect	add_vect(t_vect v1, t_vect v2)
{
	t_vect	result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return (result);
}

t_vect	multiply_vect(float scalar,t_vect vect)
{
	t_vect	res;

	res.x = vect.x * scalar;
	res.y = vect.y * scalar;
	res.z = vect.z * scalar;
	return (res);
}
