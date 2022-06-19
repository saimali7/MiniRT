#include "../inc/Vectors.h"

float		*new_vect(float x, float y, float z)
{
	float	*new;

	new = calloc(3, sizeof(float));
	if (!new)
		error_exit(-1, ERR_MEM_AL);
	new[0] = x;
    new[1] = y;
 	new[2] = z;
 	return (new);
}

float	*subtr_vec(float *vect1, float *vect2)
{
	float	*res;

 	res = new_vect(vect1[0] - vect2[0], vect1[1] - vect2[1], vect1[2] - vect2[2]);
	return (res);
}

float	length_vect(float *vect)
{
	float	res;

	res = sqrt((vect[0] * vect[0]) + (vect[1] * vect[1]) + (vect[2] * vect[2]));
	return (res);
}

void	normalize_vect(float *vect)
{
	float	length;

	length = length_vect(vect);
	vect[0] /= length;
	vect[1] /= length;
	vect[2] /= length;
}

float	dot_product_vect(float *vect1, float *vect2)
{
	float	res;

	res = ((vect1[0] * vect2[0]) + (vect1[1] * vect2[1]) + (vect1[2] * vect2[2]));
	return (res);
}

float	*add_vect(float *vect1, float *vect2)
{
	float *temp;

	temp = (float *) malloc(sizeof(float) * 3);
	temp[0] = vect1[0] + vect2[0];
	temp[1] = vect1[1] + vect2[1];
	temp[2] = vect1[2] + vect1[2];
	return (temp);
}
float	*multiply_vect(float scalar,float *vect2)
{
	float *temp;
	temp = (float *) malloc(sizeof(float) * 3);

	temp[0] = vect2[0] * scalar;
	temp[1] = vect2[1] * scalar;
	temp[2] = vect2[2] * scalar;
	return (temp);
}