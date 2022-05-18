#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

int is_sphere(t_rt *rt, float x, float y)
{
	float dist;
	float rad;

	dist = sqrtf(powf(x - rt->sphere->coord[0], 2) + powf(y - rt->sphere->coord[1], 2));
	rad = rt->sphere->diametr /2;
	if (!rt->sphere)
		return (0);
	if (dist <= rad)
	{
		if ((rad - dist) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

// int is_cylinder(t_rt *rt, float x, float y)
// {
// 	float check = 1.000000;

// 	if ((x < rt->cylinder->coord[0] ) || (y < rt->cylinder->coord[1]) || y > (rt->cylinder->coord[1] + rt->cylinder->height) || x > (rt->cylinder->coord[0] + rt->cylinder->diametr))
// 		return 0;
// 	if ((x - rt->cylinder->coord[0] < check) || (y - rt->cylinder->coord[1] < check) || (rt->cylinder->coord[1] + rt->cylinder->height - y < check) || (rt->cylinder->coord[0] + rt->cylinder->diametr - x < check))
// 		return 1;
// 	return 2;
// }

unsigned int	ft_get_rgb(int *color)
{
	unsigned int	x;

	x = 0 << 24 | color[0] << 16 | color[1] << 8 | color[2];
	return (x);
}

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

typedef struct	s_vplane
{
	float		width;
	float		hight;
	float		x_pix;
	float		y_pix;
}				t_vplane;

t_vplane	*get_view_plane(int fov)
{
	t_vplane	*new;
	float		aspect;

	new = malloc(sizeof(t_vplane));
	if (!new)
		error_exit(-1, ERR_MEM_AL);
	aspect =  WIDHT / HEIGHT;
	new->width = 10; //((tan(fov / 2)) * (M_PI / 180)) * 200; //will change
	new->hight = new->width / aspect;
	new->x_pix = new->width / WIDHT;
	new->y_pix = new->hight / HEIGHT;
	fov = 1;
	printf ("wight = %f, hight = %f, x_pix = %f, y_pix = %f\n", new->width, new->hight, new->x_pix, new->y_pix); //delete
	return (new);
}

int	exit_hook(int x)
{
	(void)x;
	//mlx_destroy_window(rt->display->mlx, rt->display->mlx_win);
	//free_all;
	exit(1);
	//return 0;
}

int	ft_key(int key, t_rt *rt)
{
	if (key == 53)
	{
		//ft_free(rt);
		exit(1);
	}
	(void)rt;
	return 0;
}

void	ft_calculate(t_disp *display, t_rt *rt)
{
	//	ray_tracing(rt);

	// int			mlx_x;
	// int			mlx_y;
	// float		ang_x;
	// float		ang_y;
	// float		ray_x;
	// float		ray_y;
	// t_vect		*ray;
	// t_vplane	*vplane; // view window

	// unsigned int color; //delete

	// mlx_y = 0;
	// vplane = get_view_plane(rt->camera.fov);
	// ang_y = HEIGHT / 2;

	// // ray = new_vect(100, 0, -1);
	// // is_ray_sphere(rt->camera, ray, rt->sphere);

	// while (ang_y >= ((HEIGHT / 2) * (-1)))
	// {
	// 	ray_y = ang_y * (vplane->y_pix);
	// 	ang_x = (WIDHT / 2) * (-1);
	// 	mlx_x = 0;
	//  	while (ang_x <= (WIDHT / 2))
	//  	{
	//  		ray_x = ang_x * (vplane->x_pix);
	// 		//my_mlx_pixel_put(&display->img, 5, mlx_y, ft_get_rgb(rt->sphere->color));
	// 		ray = new_vect(ray_x, ray_y, -1);
	//  		normalize_vect(ray);
	//  		if (is_ray_sphere(rt->camera, ray, rt->sphere))
	//  			color = ft_get_rgb(rt->sphere->color);
	//  		else
	//  			color = 0;
	//  		my_mlx_pixel_put(&display->img, 5, mlx_y, color);
	//  		free(ray);
	//  		ang_x++;
	//  		mlx_x++;
	//  	}
	// 	ang_y--;
	// 	mlx_y++;
	// }
	//my_mlx_pixel_put(&display->img, 5, 5, 0x00FFFFFFF);

	float		x;
	float		y;
	float		ray_x;
	float		ray_y;
	float		*ray;
	t_vplane	*vplane; // view window

	vplane = get_view_plane(rt->camera.fov);
	y = HEIGHT / -2 + 1;
	while (y < HEIGHT / 2)
	{
		ray_y = y * (vplane->y_pix);
		//printf ("%f ", ray_y);
		x = WIDHT / -2;
		while (x < WIDHT / 2)
		{
			//printf("%d\n", x);
			ray_x = x * (vplane->x_pix);
			ray = new_vect(ray_x * 1, ray_y * 1, -1);
			normalize_vect(ray);
			if (is_ray_sphere(rt->camera, ray, rt->sphere))
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color));
	 			//my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color));
			//if (y == 0.01 * x * x - 240)
				//my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, 0x00FFFFFFF); //for fun
			//if (x == y)
			if (is_sphere(rt, x, y) == 2)
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color)/1.09);
			//else if (is_sphere(rt, x, y))
				//my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color)/1.09);
			free(ray);
			x++;
		}
		y++;
	}
	//ray = new_vect(-100, 200, -1);
	//if (is_ray_sphere(rt->camera, ray, rt->sphere))
		//my_mlx_pixel_put(&display->img, WIDHT / 2 - 100, HEIGHT / 2 - 200, 0x0000FFFFF);

	// y = HEIGHT / -2;
	// while (y < HEIGHT / 2)
	// {
	// 	x = WIDHT / -2;
	// 	while (x < WIDHT / 2)
	// 	{
	// 		if (y == 0.01 * x * x - 240)
	// 			my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, 0x00FFFFFFF); //for fun
	// 		if (is_sphere(rt, x, y) == 2)
	// 			my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color));
	// 		else if (is_sphere(rt, x, y))
	// 			my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color)/1.01);
	// 		x++;
	// 	}
	// 	y++;
	// }

	// y = HEIGHT / -2;
	// while (y < HEIGHT / 2)
	// {
	// 	x = WIDHT / -2;
	// 	while (x < WIDHT / 2)
	// 	{
	// 		if (is_cylinder(rt, x, y) == 2)
	// 			my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->cylinder->color));
	// 		else if (is_cylinder(rt, x, y))
	// 			my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->cylinder->color) * 1.01);
	// 		x++;
	// 	}
	// 	y++;
	// }

	mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0);
	mlx_hook(display->mlx_win, 17, 0, exit_hook, 0);
	mlx_key_hook(display->mlx_win, ft_key, rt);
	mlx_loop(display->mlx);
}
