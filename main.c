#include "inc/MiniRt.h"
#include "inc/Minilibx.h"

int main(int argc, char **argv)
{
	t_rt *rt;
	rt = (t_rt *)malloc(sizeof(t_rt));
	if (!rt)
	{
		exit(1);
	}

	if (argc == 2)
	{
		printf("here\n");
		ft_putendl_fd(argv[1], 0);
		// open and read
		// parse
		// calculate
		// draw
	}
	free (rt);
	return (0);
}
