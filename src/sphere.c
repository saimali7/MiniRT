#include "../inc/MiniRt.h"
#include "../inc/Vectors.h"

float	is_ray_sphere(t_camera camera, t_vect *ray, t_sphere *sphere)
{
	float	b;
	float	c;
	float	discr;
 	float	dist1;
	float	dist2;
	t_vect	*cam_sph;

	t_vect *cam_coord = new_vect(camera.coord[0], camera.coord[1], camera.coord[2]); //change all
	t_vect *sphere_coord = new_vect(sphere->coord[0], sphere->coord[1], sphere->coord[2]); //change all
	// printf("cam_coord x = %f, y = %f, z = %f\n", cam_coord->x, cam_coord->y, cam_coord->z);
	// printf("sphere_coord x = %f, y = %f, z = %f\n", sphere_coord->x, sphere_coord->y, sphere_coord->z);
	dist1 = 0;
	dist2 = 0;
	cam_sph = subtr_vec(cam_coord, sphere_coord);
	b = 2 * dot_product_vect(cam_sph, ray);
	c = dot_product_vect(cam_sph, cam_sph) - (sphere->radius * sphere->radius);
	discr = (b * b) - (4 * c);
	free(cam_sph);
	free(cam_coord); //change all
	free(sphere_coord); //change all
	if (discr < 0) //no interseption
		return (0);
	dist1 = (b * (-1)  - sqrt(discr)) / 2; //(a * 2); a - distance before window
	dist2 = (b * (-1)  + sqrt(discr)) / 2; // (a * 2);
	//printf("dist1 = %f, dist2 = %f, discr = %f\n", dist1, dist2, discr); //delete
	if (dist1 > 0)
	{
		printf("1");
		return 1; //(dist1);
	}
	return (0);
}
