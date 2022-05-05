#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"


void	my_mlx_pixel_put(t_img *data, int x, int y, int color) //for example
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int is_circle ( float x, float y)
{
    float dist;

    dist = sqrtf(((x - 0) * (x - 0)) + ((y - 0) * (y - 0)));

    if (dist <= 142) //radius
    {
        if ((142 - dist) < 1.00000000)
            return (2);
        return (1);
    }
    return (0);
}

void    ft_calculate(t_disp *display)
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
                my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, 0x00FFFFFF); //move
            if (is_circle(x, y) == 2)
                my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, 0x00FF0000);
            else if (is_circle(x, y))
                my_mlx_pixel_put(&display->img, WIDHT / 2 + x, HEIGHT / 2 - y, 0x003F0000);
            x++;
        }
        y++;
    }   
	mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0); //move
	mlx_loop(display->mlx); //move
}
