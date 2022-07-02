#include "../../inc/MiniRt.h"

t_vect inverse_color(t_vect color)
{
	t_vect	tmp;
	float	med_val;

	med_val = 255 / 2;
	tmp.x = color.x - med_val;
	tmp.y = color.y - med_val;
	tmp.z = color.z - med_val;
	return (new_vect(med_val - tmp.x, med_val - tmp.y, med_val - tmp.z));
}

t_vect	lin_interpol(t_vect even, t_vect odd, float k)
{
	t_vect res;

	res.x = even.x * (1 - k) + odd.x * k;
	res.y = even.y * (1 - k) + odd.y * k;
	res.z = even.z * (1 - k) + odd.z * k;
	return (res);
}

t_vect chessboard(t_ray ray, t_vect even, t_vect odd, float closest_t, float koef)
{
	float	chessboard;
	float	cc1;
	t_vect	tmp_p;
	t_vect	res;

	tmp_p = multiply_vect(closest_t, ray.dir);
	tmp_p = add_vect(ray.origin, tmp_p);
	tmp_p = multiply_vect(1.0 / koef, tmp_p);
	chessboard = floorf(tmp_p.x) + floorf(tmp_p.y) + floorf(tmp_p.z); // check working in shcool
	//chessboard = round(tmp_p.x) + round(tmp_p.y) + round(tmp_p.z);
	chessboard = modff(chessboard * 0.5, &cc1) * 2;
	chessboard = fabsf(chessboard);
	res = lin_interpol(even, odd, chessboard);
	return (res);
}