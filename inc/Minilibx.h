#ifndef MINILIBX_H
# define MINILIBX_H

# include "../mlx/mlx.h"
# include "MiniRt.h"

# define WIDHT 1000
# define HEIGHT 500

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
	void			*mlx;
	void			*mlx_win;
	//unsigned int	**buffer;
	t_img			img;
}					t_disp;

#endif
