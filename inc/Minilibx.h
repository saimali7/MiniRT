#ifndef MINILIBX_H
# define MINILIBX_H

# include "../mlx/mlx.h"
# include "MiniRt.h"

# define WIDHT 1200.00
# define HEIGHT 800.00
# define INF 100000

typedef struct s_rt	t_rt;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_display
{
	void	*mlx;
	void	*mlx_win;
	//unsigned int	**buffer;
	t_img	img;
	float	width;
	float	height;
}			t_disp;

int		ft_key(int key, t_rt *rt);
int		exit_hook(int x);
void	my_mlx_pixel_put(t_img *data, int x, int y, int color);
void	ft_free_display(t_disp	*display);

#endif
