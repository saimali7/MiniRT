#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

// unsigned int	ft_get_rgb(int *color)
// {
// 	unsigned int	x;

// 	x = 0 << 24 | color[0] << 16 | color[1] << 8 | color[2];
// 	return (x);
// }

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

