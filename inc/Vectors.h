#ifndef VECTORS_H
# define VECTORS_H

#include "MiniRt.h"
#include <math.h>

typedef struct	s_vect
{
	float		x;
	float		y;
	float		z;
}				t_vect;

float	*new_vect(float x, float y, float z);
float   *subtr_vec(float *vect1, float *vect2);
float	lenght_vect(float *vect);
void	normalize_vect(float *vect);
float	dot_product_vect(float *vect1, float *vect2);
float	*cross_product(float *vect1, float *vect2);



#endif
