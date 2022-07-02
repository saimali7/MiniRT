#include "../../inc/MiniRt.h"

void	free_rt(t_rt **ptr_rt)
{
	t_rt		*rt;
	t_plane		*plane;
	t_sphere	*sphere;
	t_cylndr	*cylndr;
	t_parab		*parab;

	rt = *ptr_rt;
	// plane = NULL;
	// sphere = NULL;
	// cylndr = NULL;
	// parab = NULL;
	while (rt->plane != NULL)
	{
		plane = rt->plane;
		rt->plane = rt->plane->next;
		free (plane);
	}
	while (rt->sphere != NULL)
	{
		sphere = rt->sphere;
		rt->sphere = rt->sphere->next;
		free (sphere);
	}
	while (rt->cylndr != NULL)
	{
		cylndr = rt->cylndr;
		rt->cylndr = rt->cylndr->next;
		free (cylndr);
	}
	while (rt->parab != NULL)
	{
		parab = rt->parab;
		rt->parab = rt->parab->next;
		free (parab);
	}
	free (rt);
}

void	free_all(t_rt *ptr_rt)
{
	free_rt(&ptr_rt);
}

int	exit_hook(int x)
{
	(void)x;
	//mlx_destroy_window(rt->display->mlx, rt->display->mlx_win);
	//free_all;
	exit(1);
}

int	ft_key(int key, t_rt *rt)
{
	if (key == 53)
	{
		free_all(rt);
		exit(1);
	}
	return (0);
}

void	ft_free_display(t_disp	*display) // need paste
{
	if (display)
	{
		if (display->img.img)
			mlx_destroy_image(display->mlx, display->img.img);
		if (display->mlx_win)
			mlx_destroy_window(display->mlx, display->mlx_win);
		if (display->mlx)
			free(display->mlx);
		free(display);
	}
}