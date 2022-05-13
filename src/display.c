#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

static int	init_screen(t_disp	*display)
{
	display->img.img = mlx_new_image(display->mlx, WIDHT, HEIGHT);
	if (display->img.img)
	{
		display->img.addr = mlx_get_data_addr(display->img.img,
				&display->img.bits_per_pixel,
				&display->img.line_length,
				&display->img.endian);
		return (1);
	}
	else
	{
		//ft_putendl_fd("Error init screen", 2);
		//ft_free_display(display, 0);
		return (0);
	}
}


void	init_window(t_disp *display, t_rt *rt)
{
	display = (t_disp *)ft_calloc(sizeof(t_disp), 1);
	if (!display)
		exit(1); // change error funct("Display allocate failed")
	display->mlx = mlx_init();
	if(!display->mlx)
		exit(1); // change error funct("Init mlx failed")
	display->mlx_win = mlx_new_window(display->mlx, WIDHT, HEIGHT, "MiniRt");
	if (!display->mlx_win)
		exit(1); // change error funct("Init window failed")
	if (!init_screen(display))
		exit(1); // change error funct("Init screen failed")
	ft_calculate(display, rt);
}

void	ft_free_display(t_disp	*display) // need paste
{
	// int	xy;
	// int	j;

	//xy = 0;
	if (display)
	{
		if (display->img.img)
			mlx_destroy_image(display->mlx, display->img.img);
		if (display->mlx_win)
			mlx_destroy_window(display->mlx, display->mlx_win);
		if (display->mlx)
			free(display->mlx);

		// if (display->buffer)
		// {
		// 	j = 0;
		// 	while (j++ < xy)
		// 		free(display->buffer[j]);
		// 	free(display->buffer);
		// }
		free(display);
	}
}