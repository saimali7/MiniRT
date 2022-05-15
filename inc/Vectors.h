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

t_vect	*new_vect(float x, float y, float z);
t_vect	*subtr_vec(t_vect *vect1, t_vect *vect2);
float	lenght_vect(t_vect *vect);
void	normalize_vect(t_vect *vect);
float	dot_product_vect(t_vect *vect1, t_vect *vect2);



#endif
