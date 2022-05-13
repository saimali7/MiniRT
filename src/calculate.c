#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

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

int is_cylinder(t_rt *rt, float x, float y)
{
	float check = 1.000000;

	if ((x < rt->cylinder->coord[0] ) || (y < rt->cylinder->coord[1]) || y > (rt->cylinder->coord[1] + rt->cylinder->height) || x > (rt->cylinder->coord[0] + rt->cylinder->diametr))
		return 0;
	if ((x - rt->cylinder->coord[0] < check) || (y - rt->cylinder->coord[1] < check) || (rt->cylinder->coord[1] + rt->cylinder->height - y < check) || (rt->cylinder->coord[0] + rt->cylinder->diametr - x < check))
		return 1;
	return 2;
}

int is_plane(t_rt *rt, float x, float y)
{
	(void)rt;
	if (x || y)
		return(0);
	return(0);
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
	float x;
	float y;

	y = HEIGHT / -2;
	while (y < HEIGHT / 2)
	{
		x = WIDHT / -2;
		while (x < WIDHT / 2)
		{
			if (y == 0.01 * x * x - 240)
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, 0x00FFFFFFF); //for fun
			if (is_sphere(rt, x, y) == 2)
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color));
			else if (is_sphere(rt, x, y))
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->sphere->color)/1.01);
			x++;
		}
		y++;
	}

	y = HEIGHT / -2;
	while (y < HEIGHT / 2)
	{
		x = WIDHT / -2;
		while (x < WIDHT / 2)
		{
			if (is_cylinder(rt, x, y) == 2)
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->cylinder->color));
			else if (is_cylinder(rt, x, y))
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->cylinder->color) * 1.01);
			x++;
		}
		y++;
	}

	y = HEIGHT / -2;
	while (y < HEIGHT / 2)
	{
		x = WIDHT / -2;
		while (x < WIDHT / 2)
		{
			if (is_plane(rt, x, y) == 2)
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->plane->color));
			else if (is_plane(rt, x, y))
				my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, ft_get_rgb(rt->plane->color)/1.01);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0);
	(void)rt;
	mlx_hook(display->mlx_win, 17, 0, exit_hook, 0);
	mlx_key_hook(display->mlx_win, ft_key, rt);
	mlx_loop(display->mlx);
}
