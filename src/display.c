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
		error_exit(-1, ERR_MEM_AL);
	display->mlx = mlx_init();
	if (!display->height)
		display->height = HEIGHT;
	if (!display->width)
		display->width = WIDHT;
	if(!display->mlx)
		error_exit(-1, "Error: Init mlx failed");
	display->mlx_win = mlx_new_window(display->mlx, display->width, display->height, "MiniRt");
	if (!display->mlx_win)
		error_exit(-1, "Error: Init window failed");
	if (!init_screen(display))
		error_exit(-1, "Error: Init screen failed");
	(void)rt;
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