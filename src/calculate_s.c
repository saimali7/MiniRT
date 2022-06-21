#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

void    ft_putpixel(int x, int y, int color, t_disp *display)
{
    char	*temp;
	int		position;

    x = WIDHT/2 + x;
    y = HEIGHT/2 - y - 1;
	position = x * 4 + 4 * WIDHT * y;
	temp = display->img.addr;
	temp[position] = color % 256;
	temp[position + 1] = (color / 256) % 256;
	temp[position + 2] = color / (256 * 256);
}

float*	multiply_vectors(t_camera *cam, float *right, float *up, float *tmp)
{
	float	*res;
	float	x;
	float	y;
	float	z;

	x = tmp[0] * right[0] + tmp[1] * up[0] + tmp[2] * cam->orient[0] + cam->coord[0];
	y = tmp[0] * right[1] + tmp[1] * up[1] + tmp[2] * cam->orient[1] + cam->coord[1];
	z = tmp[0] * right[2] + tmp[1] * up[2] + tmp[2] * cam->orient[2] + cam->coord[2];
	res = new_vect(x, y, z);
	free (tmp);
	return (res);
}

float	*get_direction (int x, int y, t_rt *rt)
{
	float	fov;
	float	aspect;
	
	fov = (float)rt->camera.fov;
    aspect = WIDHT / HEIGHT;
	float new_width = (tan(fov / 2 * (M_PI / 180))) * 2;
	float new_hight = new_width / aspect;
	float x_pix = new_width / WIDHT;
	float y_pix = new_hight / HEIGHT;
	return (new_vect(x * x_pix, y * y_pix, 1.0));
}

float	*get_look_right(t_camera *camera)
{
	float	*rand;
	float	*right;

	rand = new_vect(0, 1, 0);
	normalize_vect(rand);
	right = cross_product(rand, camera->orient);
	normalize_vect(right);
	free(rand);
	return (right);
}

float	*get_look_up(t_camera *camera, float *right)
{
	float	*up;

	up = cross_product(camera->orient, right);
	normalize_vect(up);
	//printf("x = %f, y = %f, z= %f;", up[0], up[1], up[2]); // ok
	return (up);
}

float *Convert_Viewport(int x, int y, t_rt *rt)
{
    float	*direction;
	float	*to_origin;
	float	*right;
	float	*up;

	right = get_look_right(&rt->camera);
	//printf("x = %f, y = %f, z= %f;", right[0], right[1], right[2]); //ok
	up = get_look_up(&rt->camera, right);
	to_origin = multiply_vectors(&rt->camera, right, up, new_vect(0, 0, 0));
	//printf("x = %f, y = %f, z= %f;", to_origin[0], to_origin[1], to_origin[2]); //ok
	rt->camera.origin[0] = to_origin[0];
	rt->camera.origin[1] = to_origin[1];
	rt->camera.origin[2] = to_origin[2];
	direction = get_direction(x, y, rt);
	direction = multiply_vectors(&rt->camera, right, up, direction);
	direction = subtr_vec(direction, to_origin);
	//printf("x = %f, y = %f, z= %f;", direction[0], direction[1], direction[2]); //
	normalize_vect(direction);
	free(right);
	free(up);

	return (direction);
}

void    intersect_sphere(t_rt *rt, float *direction, t_sphere *sphere, float *intersect)
{
    float *oc;
	float origin[3]; //replace with actual camera cord.
    float s_center[3];
    float a;
    float b;
    float c; //saving dot product as int?
    float discriminant;
	(void) rt;

    s_center[0] = sphere->coord[0];
    s_center[1] = sphere->coord[1];
    s_center[2] = sphere->coord[2];

    origin[0] = rt->camera.origin[0];	//rt->camera.coord[0]; we can take from struct
    origin[1] = rt->camera.origin[1];	//rt->camera.coord[1];
    origin[2] = rt->camera.origin[2];	//rt->camera.coord[2];

    oc = subtr_vec(origin, s_center);
    a = dot_product_vect(direction, direction);
    b = 2 * dot_product_vect(oc, direction);
    c = dot_product_vect(oc, oc) - sphere->radius * sphere->radius;

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        intersect[0] = INF;
        intersect[1] = INF;
        return ;
    }
    intersect[0] = (-b + sqrt(discriminant)) / (2 * a);
    intersect[1] = (-b - sqrt(discriminant)) / (2 * a);
    return ;
}

int     trace_ray(t_rt *rt, float *direction, int min, int max)
{
    float closest_t = INF;
    t_sphere *closest_sphere = NULL;
    t_sphere *sphere;
    float *intersect;

    sphere = rt->sphere;
    intersect = malloc(sizeof(float) * 2);
    while (sphere != NULL)
    {
        intersect_sphere(rt, direction, sphere, intersect);
        if (intersect[0] < closest_t && min < intersect[0] && intersect[0] < max)
        {
            closest_t = intersect[0];
            closest_sphere = sphere;
        }
        if (intersect[1] < closest_t && min < intersect[1] && intersect[1] < max)
        {
            closest_t = intersect[1];
            closest_sphere = sphere;
        }
        sphere = sphere->next;
    }
    if (closest_sphere == NULL)
        return (0x000000);
    else
        return ((65536  * closest_sphere->color[0]) + (256 * closest_sphere->color[1]) + closest_sphere->color[2]);
}

void    calculate_s(t_disp *display , t_rt *rt)
{
    int x;
    int y;
    int color;
    float *direction;

    y = -HEIGHT/2;
    x = 0;
    color = 0xFFFFFF;
    while (y < HEIGHT/2)
    {
        x = -WIDHT/2;
        while (x < WIDHT/2)
        {
            direction = Convert_Viewport(x, y, rt);
            //color = trace_ray(rt, direction, 1 , INF);
			//color = trace_ray_plane(rt, direction);
			color = trace_ray_cylinder(rt, direction);
            ft_putpixel(x, y , color, display);
            free(direction);
            x++;
        }
        y++;
    }
    mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0);
	mlx_hook(display->mlx_win, 17, 0, exit_hook, 0);
	mlx_key_hook(display->mlx_win, ft_key, rt);
	mlx_loop(display->mlx);
}
