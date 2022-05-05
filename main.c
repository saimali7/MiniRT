#include "inc/MiniRt.h"
#include "inc/Minilibx.h"

t_plane *new_plane(void)
{
	t_plane *ptr;

	ptr = (t_plane *) malloc(sizeof(t_plane));
	if (!ptr)
		return (NULL);
	ptr->next = NULL;
	return (ptr);
}

t_sphere *new_sphere(void)
{
	t_sphere *ptr;

	ptr = (t_sphere *) malloc(sizeof(t_sphere));
	if (!ptr)
		return (NULL);
	ptr->next = NULL;
	return (ptr);
}

t_cylinder *new_cylinder(void)
{
	t_cylinder *ptr;

	ptr = (t_cylinder *) malloc(sizeof(t_cylinder));
	if (!ptr)
		return (NULL);
	ptr->next = NULL;
	return (ptr);
}

t_rt	*init_rt(void)
{
	t_rt *ptr;

	ptr = ft_calloc(sizeof(t_rt), 1);
	if (!ptr)
		return (NULL);
	ptr->plane = NULL;
	ptr->sphere = NULL;
	ptr->cylind = NULL;
	ptr->display = NULL;
	return (ptr);
}

void	free_rt(t_rt **ptr_rt)
{
	t_rt	*rt;
	t_plane *plane;
	t_sphere *sphere;
	t_cylinder *cylinder;

	rt = *ptr_rt;
	plane = NULL;
	sphere = NULL;
	cylinder = NULL;
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
	while (rt->cylind != NULL)
	{
		cylinder = rt->cylind;
		rt->cylind = rt->cylind->next;
		free (cylinder);
	}
	free (rt);
}

int		extension_check(char *arg)
{
	int i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '.')
		{
			if (ft_strncmp(".rt", (arg + i), 4) != 0)
				return (-1);
		}
		i++;
	}
	return (0);
}

int		open_and_check(char *arg)
{
	int fd;
	int	rd;
	char *buf;

	fd = 0;
	rd = 0;
	buf = (char *) malloc(sizeof(char));
	fd = open(arg, O_RDONLY);
	if (fd == -1) //check is not folder
		return (-1);
	rd = read(fd, buf, 1);
	if (rd < 0)
	{
		free(buf);
		close (fd);
		return (-1);
	}
	close(fd);
	fd = open(arg, O_RDONLY);
	free(buf);
	return (fd);
}

double	ft_value(char *str, char delimeter, int reset)
{
	int k;
	static int i;
	char *sub;
	double value;

	if (reset == 0)
		i = 0;
	if (str[i] == delimeter || str[i] == ',')
		i = i + 1;
	k = i;
	while (str[i] != '\0' && str[i] != delimeter && str[i] != '\n')
		i = i + 1;
	sub = ft_substr(str, k, i - k);
	value = ft_atod(sub); // change rt_atod with t_rt
	free (sub);
	return (value);
}

int		comma_helper(char *str, int *i)
{
	while (str[*i] != ',' && str[*i] != '\0' && str[*i] != '\n')
	{
		if (ft_isdigit(str[*i]) != 1 && str[*i] != '.' && str[*i] != '-' && str[*i] != '+')
			return (-1);
		*i = *i + 1;
	}
	if (str[*i] != ',' && str[*i] != '\0' && str[*i] != '\n')
		return (-1);
	if (str[*i + 1] != '+' && str[*i + 1] != '-' && ft_isdigit(str[*i + 1]) != 1 && str[*i] != '\0' && str[*i] != '\n')
		return (-1);
	return (0);
}

int		comma_check(char *str)
{
	int i;

	i = 0;
	if (comma_helper(str, &i) == -1)
		return (-1);
	i++;
	if (comma_helper(str, &i) == -1)
		return (-1);
	i++;
	if (comma_helper(str, &i) == -1)
		return (-1);
	return (0);
}

int		set_ambient(char **line, t_rt *rt)
{
	rt->ambient.ratio = rt_atod(line[1], rt);
	if (rt->ambient.ratio < 0.0 || rt->ambient.ratio > 1.0)
		return (-1);
	if (comma_check(line[2]) == -1)
		return (-1);
	rt->ambient.color[0] = ft_value(line[2], ',', 0);
	rt->ambient.color[1] = ft_value(line[2], ',', 1);
	rt->ambient.color[2] = ft_value(line[2], ',', 1);
	if (rt->ambient.color[0] < 0 || rt->ambient.color[0] > 255 || rt->ambient.color[1] < 0
	|| rt->ambient.color[1] > 255 || rt->ambient.color[2] < 0 || rt->ambient.color[2] > 255)
		return (-1);
	return (0);
}

int		array_size(char **line)
{
	int i;

	i = 0;
	while (line[i] != NULL && line[i][0] != '\n')
		i++;
	return (i);
}

int		alpha_check(char **line)
{
	int i;
	int k;

	i = 1;
	while (line[i] != NULL)
	{
		k = 0;
		while (line[i][k] != '\0')
		{
			if ((ft_isdigit(line[i][k]) != 1) && line[i][k] != '.' && line[i][k] != '+' && line[i][k] != '-' && line[i][k] != ',' && line[i][k] != '\n')
				return (-1);
			k++;
		}
		i++;
	}
	return (0);
}

int		parse_ambient(char **line, t_rt *rt)
{
	int			size;
	static int	a;

	a++;
	if (a > 1)
		return (-1);
	size = array_size(line);
	if (size != 3)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_ambient(line, rt) == -1)
		return (-1);
	return (0);
}

int		set_camera(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1)
		return (-1);
	rt->camera.coord[0] = ft_value(line[1], ',', 0);
	rt->camera.coord[1] = ft_value(line[1], ',', 1);
	rt->camera.coord[2] = ft_value(line[1], ',', 1);
	rt->camera.orient[0] = ft_value(line[2], ',', 0);
	rt->camera.orient[1] = ft_value(line[2], ',', 1);
	rt->camera.orient[2] = ft_value(line[2], ',', 1);
	rt->camera.fov = ft_value(line[3], ',', 0);
	if (rt->camera.coord[0] < -100 || rt->camera.coord[0] > 100 || rt->camera.coord[1] < -100
	|| rt->camera.coord[1] > 100 || rt->camera.coord[2] < -100 || rt->camera.coord[2] > 100)
		return (-1);
	if (rt->camera.orient[0] < -1.0 || rt->camera.orient[0] > 1.0 || rt->camera.orient[1] < -1.0
	|| rt->camera.orient[1] > 1.0 || rt->camera.orient[2] < -1.0 || rt->camera.orient[2] > 1.0)
		return (-1);
	if (rt->camera.fov < 0 || rt->camera.fov > 180)
		return (-1);
	return (0);
}

int		parse_camera(char **line, t_rt *rt)
{
	int		size;
	static int c;

	c++;
	if (c > 1)
		return (-1);
	size = array_size(line);
	if (size != 4)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_camera(line, rt) == -1)
		return (-1);
	return (0);
}

int		set_light(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1)
		return (-1);
	if (comma_check(line[3]) == -1)
		return (-1);
	rt->light.coord[0] = ft_value(line[1], ',', 0);
	rt->light.coord[1] = ft_value(line[1], ',', 1);
	rt->light.coord[2] = ft_value(line[1], ',', 1);
	rt->light.ratio = ft_value(line[2], ',', 0);
	rt->light.color[0] = ft_value(line[3], ',', 0);
	rt->light.color[1] = ft_value(line[3], ',', 1);
	rt->light.color[2] = ft_value(line[3], ',', 1);
	if (rt->light.color[0] < 0 || rt->light.color[0] > 255 || rt->light.color[1] < 0
	|| rt->light.color[1] > 255 || rt->light.color[2] < 0 || rt->light.color[2] > 255)
		return (-1);
	if (rt->light.coord[0] < -100 || rt->light.coord[0] > 100 || rt->light.coord[1] < -100
	|| rt->light.coord[1] > 100 || rt->light.coord[2] < -100 || rt->light.coord[2] > 100)
		return (-1);
	if (rt->light.ratio < 0.0 || rt->light.ratio > 1.0)
		return (-1);
	return (0);
}

int		parse_light(char **line, t_rt *rt)
{
	int		size;
	static int l;

	l++;
	if (l > 1)
		return (-1);
	size = array_size(line);
	if (size != 4)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_light(line, rt) == -1)
		return (-1);
	return (0);
}

int		set_sphere(char **line,t_rt *rt)
{
	if (comma_check(line[1]) == -1 || comma_check(line[3]) == -1)
		return (-1);
	rt->sphere->coord[0] = ft_value(line[1], ',', 0);
	rt->sphere->coord[1] = ft_value(line[1], ',', 1);
	rt->sphere->coord[2] = ft_value(line[1], ',', 1);
	rt->sphere->diametr = ft_value(line[2], ',', 0);
	rt->sphere->color[0] = ft_value(line[3], ',', 0);
	rt->sphere->color[1] = ft_value(line[3], ',', 1);
	rt->sphere->color[2] = ft_value(line[3], ',', 1);
	if (rt->sphere->color[0] < 0 || rt->sphere->color[0] > 255 || rt->sphere->color[1] < 0
	|| rt->sphere->color[1] > 255 || rt->sphere->color[2] < 0 || rt->sphere->color[2] > 255)
		return (-1);
	if (rt->sphere->coord[0] < -100 || rt->sphere->coord[0] > 100 || rt->sphere->coord[1] < -100
	|| rt->sphere->coord[1] > 100 || rt->sphere->coord[2] < -100 || rt->sphere->coord[2] > 100)
		return (-1);
	if (rt->sphere->diametr < 0 || rt->sphere->diametr > 100)
		return(-1);
	return (0);
}

int		parse_sphere(char **line, t_rt *rt)
{
	int		size;
	t_sphere *temp;

	temp = rt->sphere;
	while (rt->sphere != NULL)
		rt->sphere = rt->sphere->next;
	rt->sphere = new_sphere();
	size = array_size(line);
	if (size != 4)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_sphere(line, rt) == -1)
		return (-1);
	if (temp != NULL)
		rt->sphere = temp;
	return (0);
}

int		set_plane(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1 || comma_check(line[2]) == -1 || comma_check(line[2]) == -1)
		return (-1);
	rt->plane->coord[0] = ft_value(line[1], ',', 0);
	rt->plane->coord[1] = ft_value(line[1], ',', 1);
	rt->plane->coord[2] = ft_value(line[1], ',', 1);
	rt->plane->orient[0] = ft_value(line[2], ',', 0);
	rt->plane->orient[1] = ft_value(line[2], ',', 1);
	rt->plane->orient[2] = ft_value(line[2], ',', 1);
	rt->plane->color[0] = ft_value(line[3], ',', 0);
	rt->plane->color[1] = ft_value(line[3], ',', 1);
	rt->plane->color[2] = ft_value(line[3], ',', 1);
	if (rt->plane->coord[0] < -100 || rt->plane->coord[0] > 100 || rt->plane->coord[1] < -100
	|| rt->plane->coord[1] > 100 || rt->plane->coord[2] < -100 || rt->plane->coord[2] > 100)
		return (-1);
	if (rt->plane->orient[0] < -1.0 || rt->plane->orient[0] > 1.0 || rt->plane->orient[1] < -1.0
	|| rt->plane->orient[1] > 1.0 || rt->plane->orient[2] < -1.0 || rt->plane->orient[2] > 1.0)
		return (-1);
	if (rt->plane->color[0] < 0 || rt->plane->color[0] > 255 || rt->plane->color[1] < 0
	|| rt->plane->color[1] > 255 || rt->plane->color[2] < 0 || rt->plane->color[2] > 255)
		return (-1);
	return (0);
}

int		parse_plane(char **line, t_rt *rt)
{
	int		size;
	t_plane *temp;

	temp = rt->plane;
	while (rt->plane != NULL)
		rt->plane = rt->plane->next;
	rt->plane = new_plane();
	size = array_size(line);
	if (size != 4)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_plane(line, rt) == -1)
		return (-1);
	if (temp != NULL)
		rt->plane = temp;
	return (0);
}

int		cylinder_errorcheck(t_rt *rt)
{
	if (rt->cylind->coord[0] < -100 || rt->cylind->coord[0] > 100 || rt->cylind->coord[1] < -100
	|| rt->cylind->coord[1] > 100 || rt->cylind->coord[2] < -100 || rt->cylind->coord[2] > 100)
		return (-1);
	if (rt->cylind->orient[0] < -1.0 || rt->cylind->orient[0] > 1.0 || rt->cylind->orient[1] < -1.0
	|| rt->cylind->orient[1] > 1.0 || rt->cylind->orient[2] < -1.0 || rt->cylind->orient[2] > 1.0)
		return (-1);
	if (rt->cylind->rgb[0] < 0 || rt->cylind->rgb[0] > 255 || rt->cylind->rgb[1] < 0
	|| rt->cylind->rgb[1] > 255 || rt->cylind->rgb[2] < 0 || rt->cylind->rgb[2] > 255)
		return (-1);
	if (rt->cylind->diametr < 0 || rt->cylind->diametr > 100)
		return (-1);
	if (rt->cylind->height < 0 || rt->cylind->height > 100)
		return (-1);
	return (0);
}

int		set_cylinder(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1 || comma_check(line[2]) == -1 || comma_check(line[5]) == -1)
		return (-1);
	rt->cylind->coord[0] = ft_value(line[1], ',', 0);
	rt->cylind->coord[1] = ft_value(line[1], ',', 1);
	rt->cylind->coord[2] = ft_value(line[1], ',', 1);
	rt->cylind->orient[0] = ft_value(line[2], ',', 0);
	rt->cylind->orient[1] = ft_value(line[2], ',', 1);
	rt->cylind->orient[2] = ft_value(line[2], ',', 1);
	rt->cylind->diametr = ft_value(line[3], ',', 0);
	rt->cylind->height = ft_value(line[4], ',', 0);
	rt->cylind->rgb[0] = ft_value(line[5], ',', 0);
	rt->cylind->rgb[1] = ft_value(line[5], ',', 1);
	rt->cylind->rgb[2] = ft_value(line[5], ',', 1);
	if (cylinder_errorcheck(rt) == -1)
		return (-1);
	return (0);
}

int		parse_cylinder(char **line, t_rt *rt)
{
	int		size;
	t_cylinder *temp;

	temp = rt->cylind;
	while (rt->cylind != NULL)
		rt->cylind = rt->cylind->next;
	rt->cylind = new_cylinder();
	size = array_size(line);
	if (size != 6)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_cylinder(line, rt) == -1)
		return (-1);
	if (temp != NULL)
		rt->cylind = temp;
	return (0);
}

void	free_split(char **line)
{
	int i;

	i = 0;
	while (line[i] != NULL)
	{
		free (line[i]);
		i++;
	}
	free (line[i]);
}

int		parse_line(char *str, t_rt *rt)
{
	char **line;

	line = ft_split(str, ' ');
	if (ft_strncmp(line[0], "A", 2) == 0)
		return (parse_ambient(line, rt));
	else if (ft_strncmp(line[0], "C", 2) == 0)
		return (parse_camera(line, rt));
	else if (ft_strncmp(line[0], "L", 2) == 0)
		return (parse_light(line, rt));
	else if (ft_strncmp(line[0], "sp", 3) == 0)
		return (parse_sphere(line, rt));
	else if (ft_strncmp(line[0], "pl", 3) == 0)
		return (parse_plane(line, rt));
	else if (ft_strncmp(line[0], "cy", 3) == 0)
		return  (parse_cylinder(line, rt));
	else if (ft_strncmp(line[0], "\n", 2) != 0)
		return (-1);
	free_split(line);
	return (0);
}

char	*fixed_line(char *str)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = str;
	while (tmp && tmp[i])
	{
		if (ft_isspace(tmp[i]))
			tmp[i] = ' ';
		i++;
	}
	str = ft_strtrim(tmp, " ");
	free(tmp);
	return (str);
}

int		parse_set_rt(t_rt *rt, int fd)
{
	char *str;

	str = get_next_line(fd);
	while (str != NULL)
	{
		str = fixed_line(str);
		if (parse_line(str, rt) == -1)
		{
			free(str);
			return (-1);
		}
		free(str);
		str = get_next_line(fd);
	}
	free (str);
	return (0);
}

int	parse(t_rt *rt, char *arg)
{
	int fd;

	fd = 0;
	if (extension_check(arg) == -1)
		return (-1);
	fd = open_and_check(arg);
	if (fd == -1)
		return (-1);
	if (parse_set_rt(rt, fd) == -1)
		return (-1);
	close (fd);
	return (0);
}

void	print_rt(t_rt *rt)
{
	printf ("Ambient: %f	%i,%i,%i\n", rt->ambient.ratio, rt->ambient.color[0], rt->ambient.color[1], rt->ambient.color[2]);
	printf ("Camera: %f,%f,%f	%f,%f,%f %d\n", rt->camera.coord[0], rt->camera.coord[1], rt->camera.coord[2], rt->camera.orient[0], rt->camera.orient[1], rt->camera.orient[2], rt->camera.fov);
	printf ("Light: %f,%f,%f	%f		%d,%d,%d\n", rt->light.coord[0], rt->light.coord[1], rt->light.coord[2], rt->light.ratio,rt->light.color[0], rt->light.color[1], rt->light.color[2]);
	if (rt->sphere != NULL)
		printf("Sphere: %f,%f,%f %f		%d,%d,%d\n", rt->sphere->coord[0], rt->sphere->coord[1], rt->sphere->coord[2], rt->sphere->diametr, rt->sphere->color[0], rt->sphere->color[1], rt->sphere->color[2]);
	if (rt->plane != NULL)
		printf("Plane: %f,%f,%f %f,%f,%f	%d,%d,%d\n", rt->plane->coord[0], rt->plane->coord[1], rt->plane->coord[2], rt->plane->orient[0], rt->plane->orient[1], rt->plane->orient[2], rt->plane->color[0], rt->plane->color[1], rt->plane->color[2]);
	if (rt->cylind != NULL)
		printf("Cylinder: %f,%f,%f		%f,%f,%f		%f %f 	%d,%d,%d\n", rt->cylind->coord[0], rt->cylind->coord[1], rt->cylind->coord[2], rt->cylind->orient[0], rt->cylind->orient[1], rt->cylind->orient[2], rt->cylind->diametr, rt->cylind->height, rt->cylind->rgb[0], rt->cylind->rgb[1], rt->cylind->rgb[2]);
}

int main(int argc, char **argv)
{
	t_rt *rt;

	rt = init_rt();
	if (!rt)
		exit(1);
	if (argc == 2)
	{
		ft_putendl_fd(argv[1], 0);
		if (parse(rt, argv[1]) == -1)
		{
			write(2, "Error\nInvalid argument file\n", 29);
			free_rt(&rt);
			exit(-1);
		}
		print_rt(rt);
		init_window(rt->display); //will add free
		//ft_calculate(rt->display);
		// draw
	}
	else
	{
		ft_putstr_fd("Error: ", 2); // create funct error with massage, free and exit
		ft_putstr_fd("Usage: ./miniRT path_of_scene.rt", 2);
		ft_putendl_fd("", 2);
	}
	free_rt(&rt);
	return (0);
}
