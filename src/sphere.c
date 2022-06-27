#include "../inc/MiniRt.h"
#include "../inc/Vector.h"

// float	is_ray_sphere(t_camera camera, float *ray, t_sphere *sphere)
// {
// 	float	b;
// 	float	c;
// 	float	discr;
//  	float	dist1;
// 	float	dist2;
// 	float	*cam_sph;

// 	dist1 = 0;
// 	dist2 = 0;
// 	cam_sph = subtr_vec(camera.coord, sphere->coord);
// 	b = 2 * dot_product_vect(cam_sph, ray);
// 	c = dot_product_vect(cam_sph, cam_sph) - (sphere->radius * sphere->radius);
// 	discr = (b * b) - (4 * c);
// 	free(cam_sph);
// 	if (discr < 0) //no interseption
// 		return (0);
// 	dist1 = (b * (-1)  - sqrt(discr)) / 2; //(a * 2); a - distance before window
// 	dist2 = (b * (-1)  + sqrt(discr)) / 2; // (a * 2);
// 	//printf("dist1 = %f, dist2 = %f, discr = %f, radius = %f\n", dist1, dist2, discr, sphere->radius); //delete
// 	if (dist1 > 0) // || dist2 > 0)
// 	{
// 		//printf("1");
// 		return 1; //(dist1);
// 	}
// 	return (0);
// }
