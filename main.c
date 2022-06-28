#include "inc/MiniRt.h"
#include "inc/Minilibx.h"

t_rt	*init_rt(void)
{
	t_rt	*ptr;

	ptr = ft_calloc(sizeof(t_rt), 1);
	if (!ptr)
		error_exit(-1, ERR_MEM_AL);
	ptr->plane = NULL;
	ptr->sphere = NULL;
	ptr->cylinder = NULL;
	ptr->display = NULL;
	return (ptr);
}

void	print_rt(t_rt *rt)		//DELETE
{
	printf ("Ambient: %f %f,%f,%f\n", rt->ambient.ratio, rt->ambient.color.x, rt->ambient.color.y, rt->ambient.color.z);
	printf ("Camera: %f,%f,%f	%f,%f,%f %d\n", rt->camera.coord.x, rt->camera.coord.y, rt->camera.coord.z, rt->camera.orient.x, rt->camera.orient.y, rt->camera.orient.z, rt->camera.fov);
	printf ("Light: %f,%f,%f %f %f,%f,%f\n", rt->light.coord.x, rt->light.coord.y, rt->light.coord.z, rt->light.ratio,rt->light.color.x, rt->light.color.y, rt->light.color.z);
	while (rt->sphere != NULL)
	{
		printf("Sphere: %f,%f,%f %f %f,%f,%f\n", rt->sphere->coord.x, rt->sphere->coord.y, rt->sphere->coord.z, rt->sphere->diametr, rt->sphere->color.x, rt->sphere->color.y, rt->sphere->color.z);
		rt->sphere = rt->sphere->next;
	}
	while (rt->plane != NULL)
	{
		printf("Plane: %f,%f,%f %f,%f,%f %f,%f,%f\n", rt->plane->coord.x, rt->plane->coord.y, rt->plane->coord.z, rt->plane->orient.x, rt->plane->orient.y, rt->plane->orient.z, rt->plane->color.x, rt->plane->color.y, rt->plane->color.z);
		rt->plane = rt->plane->next;
	}
	while (rt->cylinder != NULL)
	{
		printf("Cylinder: %f,%f,%f %f,%f,%f %f %f %f,%f,%f\n", rt->cylinder->coord.x, rt->cylinder->coord.y, rt->cylinder->coord.z, rt->cylinder->orient.x, rt->cylinder->orient.y, rt->cylinder->orient.z, rt->cylinder->radius, rt->cylinder->height, rt->cylinder->color.x, rt->cylinder->color.y, rt->cylinder->color.z);
		rt->cylinder = rt->cylinder->next;
	}
}


int	main(int argc, char **argv)
{
	t_rt	*rt;

	rt = init_rt();
	if (!rt)
		exit(1);
	if (argc == 2)
	{
		if (parse(rt, argv[1]) == -1)
		{
			free_rt(&rt);
			error_exit(-1, "Error: Invalid argument file\n");
		}
		init_window(rt->display, rt);
		print_rt(rt);	//DELETE
	}
	else
		error_exit(-1, "Error: Usage: ./miniRT path_of_scene.rt\n");
	free_all(rt);
	return (0);
}
