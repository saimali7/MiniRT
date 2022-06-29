#include "../inc/MiniRt.h"

t_plane	*new_plane(void)
{
	t_plane	*ptr;

	ptr = (t_plane *) malloc(sizeof(t_plane));
	if (!ptr)
		error_exit(-1, ERR_MEM_AL);
	ptr->next = NULL;
	return (ptr);
}

t_sphere	*new_sphere(void)
{
	t_sphere	*ptr;

	ptr = (t_sphere *) malloc(sizeof(t_sphere));
	if (!ptr)
		error_exit(-1, ERR_MEM_AL);
	ptr->next = NULL;
	return (ptr);
}

t_cylinder	*new_cylinder(void)
{
	t_cylinder	*ptr;

	ptr = (t_cylinder *) malloc(sizeof(t_cylinder));
	if (!ptr)
		error_exit(-1, ERR_MEM_AL);
	ptr->next = NULL;
	return (ptr);
}

int	extension_check(char *arg)
{
	int	i;

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

int	open_and_check(char *arg)
{
	int		fd;
	int		rd;
	char	*buf;

	fd = 0;
	rd = 0;
	buf = (char *) malloc(sizeof(char));
	if (!buf)
		error_exit(-1, ERR_MEM_AL);
	fd = open(arg, O_RDONLY);
	if (fd == -1)
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

double	ft_value(char *str, char delimeter, int reset, t_rt *rt)
{
	int			k;
	static int	i;
	char		*sub;
	double		value;

	if (reset == 0)
		i = 0;
	if (str[i] == delimeter || str[i] == ',')
		i = i + 1;
	k = i;
	while (str[i] != '\0' && str[i] != delimeter && str[i] != '\n')
		i = i + 1;
	sub = ft_substr(str, k, i - k);
	value = rt_atod(sub, rt);
	free (sub);
	return (value);
}

int		comma_helper(char *str, int *i)
{
	while (str[*i] != ',' && str[*i] != '\0' && str[*i] != '\n')
	{
		if (ft_isdigit(str[*i]) != 1 && str[*i] != '.' \
		&& str[*i] != '-' && str[*i] != '+')
			return (-1);
		*i = *i + 1;
	}
	if (str[*i] != ',' && str[*i] != '\0' && str[*i] != '\n')
		return (-1);
	if (str[*i + 1] != '+' && str[*i + 1] != '-' && \
		ft_isdigit(str[*i + 1]) != 1 && str[*i] != '\0' && str[*i] != '\n')
		return (-1);
	return (0);
}

int	comma_check(char *str)
{
	int	i;

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

int	set_ambient(char **line, t_rt *rt)
{
	rt->ambient.ratio = rt_atod(line[1], rt);
	if (rt->ambient.ratio < 0.0 || rt->ambient.ratio > 1.0)
		return (-1);
	if (comma_check(line[2]) == -1)
		return (-1);
	rt->ambient.color.x = ft_value(line[2], ',', 0, rt);
	rt->ambient.color.y = ft_value(line[2], ',', 1, rt);
	rt->ambient.color.z = ft_value(line[2], ',', 1, rt);
	if (rt->ambient.color.x < 0 || rt->ambient.color.x > 255 || \
	rt->ambient.color.y < 0 || rt->ambient.color.y > 255 || \
	rt->ambient.color.z < 0 || rt->ambient.color.z > 255)
		return (-1);
	return (0);
}

int	array_size(char **line)
{
	int	i;

	i = 0;
	while (line[i] != NULL && line[i][0] != '\n')
		i++;
	return (i);
}

int	alpha_check(char **line)
{
	int	i;
	int	k;

	i = 1;
	while (line[i] != NULL)
	{
		k = 0;
		while (line[i][k] != '\0')
		{
			if ((ft_isdigit(line[i][k]) != 1) && line[i][k] != '.' \
					&& line[i][k] != '+' && line[i][k] != '-' \
					&& line[i][k] != ',' && line[i][k] != '\n')
				return (-1);
			k++;
		}
		i++;
	}
	return (0);
}

int	parse_ambient(char **line, t_rt *rt)
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

int	set_camera(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1)
		return (-1);
	rt->camera.coord.x = ft_value(line[1], ',', 0, rt);
	rt->camera.coord.y = ft_value(line[1], ',', 1, rt);
	rt->camera.coord.z = ft_value(line[1], ',', 1, rt);
	rt->camera.orient.x = ft_value(line[2], ',', 0, rt);
	rt->camera.orient.y = ft_value(line[2], ',', 1, rt);
	rt->camera.orient.z = ft_value(line[2], ',', 1, rt);
	rt->camera.fov = ft_value(line[3], ',', 0, rt);
	if (rt->camera.coord.x < -100 || rt->camera.coord.x > 100 || \
	rt->camera.coord.y < -100 || rt->camera.coord.y > 100 || \
	rt->camera.coord.z < -100 || rt->camera.coord.z > 100)
		return (-1);
	if (rt->camera.orient.x < -1.0 || rt->camera.orient.x > 1.0 || \
	rt->camera.orient.y < -1.0 || rt->camera.orient.y > 1.0 \
	|| rt->camera.orient.z < -1.0 || rt->camera.orient.z > 1.0)
		return (-1);
	if (rt->camera.fov < 0 || rt->camera.fov > 180)
		return (-1);
	normalize_vect(&rt->camera.orient);
	return (0);
}

int	parse_camera(char **line, t_rt *rt)
{
	int			size;
	static int	c;

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

int	set_light(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1)
		return (-1);
	if (comma_check(line[3]) == -1)
		return (-1);
	rt->light.coord.x = ft_value(line[1], ',', 0, rt);
	rt->light.coord.y = ft_value(line[1], ',', 1, rt);
	rt->light.coord.z = ft_value(line[1], ',', 1, rt);
	rt->light.ratio = ft_value(line[2], ',', 0, rt);
	rt->light.color.x = ft_value(line[3], ',', 0, rt);
	rt->light.color.y = ft_value(line[3], ',', 1, rt);
	rt->light.color.z = ft_value(line[3], ',', 1, rt);
	if (rt->light.color.x < 0 || rt->light.color.x > 255 || \
	rt->light.color.y < 0 || rt->light.color.y > 255 || \
	rt->light.color.z < 0 || rt->light.color.z > 255)
		return (-1);
	if (rt->light.coord.x < -100 || rt->light.coord.x > 100 || \
	rt->light.coord.y < -100 || rt->light.coord.y > 100 || \
	rt->light.coord.z < -100 || rt->light.coord.z > 100)
		return (-1);
	if (rt->light.ratio < 0.0 || rt->light.ratio > 1.0)
		return (-1);
	return (0);
}

int	parse_light(char **line, t_rt *rt)
{
	int			size;
	static int	l;

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

int	set_sphere(char **line,t_rt *rt)
{
	if (comma_check(line[1]) == -1 || comma_check(line[3]) == -1)
		return (-1);
	rt->sphere->coord.x = ft_value(line[1], ',', 0, rt);
	rt->sphere->coord.y = ft_value(line[1], ',', 1, rt);
	rt->sphere->coord.z = ft_value(line[1], ',', 1, rt);
	rt->sphere->diametr = ft_value(line[2], ',', 0, rt);
	rt->sphere->radius = rt->sphere->diametr / 2.0; // check using diametr
	rt->sphere->color.x = ft_value(line[3], ',', 0, rt);
	rt->sphere->color.y = ft_value(line[3], ',', 1, rt);
	rt->sphere->color.z = ft_value(line[3], ',', 1, rt);
	if (rt->sphere->color.x < 0 || rt->sphere->color.x > 255 || rt->sphere->color.y < 0
	|| rt->sphere->color.y > 255 || rt->sphere->color.z < 0 || rt->sphere->color.z > 255)
		return (-1);
	if (rt->sphere->coord.x < -MAX_SIZE || rt->sphere->coord.x > MAX_SIZE || rt->sphere->coord.y < -MAX_SIZE
	|| rt->sphere->coord.y > MAX_SIZE || rt->sphere->coord.z < -MAX_SIZE || rt->sphere->coord.z > MAX_SIZE)
		return (-1);
	if (rt->sphere->diametr < 0 || rt->sphere->diametr > MAX_SIZE)
		return(-1);
	return (0);
}

int	parse_sphere(char **line, t_rt *rt)
{
	int			size;
	t_sphere	*temp;

	temp = rt->sphere;
	while (rt->sphere != NULL && rt->sphere->next != NULL)
		rt->sphere = rt->sphere->next;
	if (rt->sphere == NULL)
		rt->sphere = new_sphere();
	else
	{
		rt->sphere->next = new_sphere();
		rt->sphere = rt->sphere->next;
	}
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

int	set_plane(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1 || comma_check(line[2]) == -1 \
		|| comma_check(line[2]) == -1)
		return (-1);
	rt->plane->coord.x = ft_value(line[1], ',', 0, rt);
	rt->plane->coord.y = ft_value(line[1], ',', 1, rt);
	rt->plane->coord.z = ft_value(line[1], ',', 1, rt);
	rt->plane->orient.x = ft_value(line[2], ',', 0, rt);
	rt->plane->orient.y = ft_value(line[2], ',', 1, rt);
	rt->plane->orient.z = ft_value(line[2], ',', 1, rt);
	normalize_vect(&rt->plane->orient);
	rt->plane->color.x = ft_value(line[3], ',', 0, rt);
	rt->plane->color.y = ft_value(line[3], ',', 1, rt);
	rt->plane->color.z = ft_value(line[3], ',', 1, rt);
	if (rt->plane->coord.x < -100 || rt->plane->coord.x > 100 || \
	rt->plane->coord.y < -100 || rt->plane->coord.y > 100 || \
	rt->plane->coord.z < -100 || rt->plane->coord.z > 100)
		return (-1);
	if (rt->plane->orient.x < -1.0 || rt->plane->orient.x > 1.0 || \
	rt->plane->orient.y < -1.0 || rt->plane->orient.y > 1.0 || \
	rt->plane->orient.z < -1.0 || rt->plane->orient.z > 1.0)
		return (-1);
	if (rt->plane->color.x < 0 || rt->plane->color.x > 255 || \
	rt->plane->color.y < 0 || rt->plane->color.y > 255 || \
	rt->plane->color.z < 0 || rt->plane->color.z > 255)
		return (-1);
	return (0);
}

int	parse_plane(char **line, t_rt *rt)
{
	int		size;
	t_plane	*temp;

	temp = rt->plane;
	while (rt->plane != NULL && rt->plane->next != NULL)
		rt->plane = rt->plane->next;
	if (rt->plane == NULL)
		rt->plane = new_plane();
	else
	{
		rt->plane->next = new_plane();
		rt->plane = rt->plane->next;
	}
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

int	cylinder_errorcheck(t_rt *rt)
{
	if (rt->cylinder->coord.x < -100 || rt->cylinder->coord.x > 100 || \
	rt->cylinder->coord.y < -100 || rt->cylinder->coord.y > 100 || \
	rt->cylinder->coord.z < -100 || rt->cylinder->coord.z > 100)
		return (-1);
	if (rt->cylinder->orient.x < -1.0 || rt->cylinder->orient.x > 1.0 || \
	rt->cylinder->orient.y < -1.0 || rt->cylinder->orient.y > 1.0 || \
	rt->cylinder->orient.z < -1.0 || rt->cylinder->orient.z > 1.0)
		return (-1);
	if (rt->cylinder->color.x < 0 || rt->cylinder->color.x > 255 || \
	rt->cylinder->color.y < 0 || rt->cylinder->color.y > 255 || \
	rt->cylinder->color.z < 0 || rt->cylinder->color.z > 255)
		return (-1);
	if (rt->cylinder->radius < 0 || rt->cylinder->radius > 100)
		return (-1);
	if (rt->cylinder->height < 0 || rt->cylinder->height > 100)
		return (-1);
	return (0);
}

int	set_cylinder(char **line, t_rt *rt)
{
	if (comma_check(line[1]) == -1 || comma_check(line[2]) == -1 \
	|| comma_check(line[5]) == -1)
		return (-1);
	rt->cylinder->coord.x = ft_value(line[1], ',', 0, rt);
	rt->cylinder->coord.y = ft_value(line[1], ',', 1, rt);
	rt->cylinder->coord.z = ft_value(line[1], ',', 1, rt);
	rt->cylinder->orient.x = ft_value(line[2], ',', 0, rt);
	rt->cylinder->orient.y = ft_value(line[2], ',', 1, rt);
	rt->cylinder->orient.z = ft_value(line[2], ',', 1, rt);
	normalize_vect(&rt->cylinder->orient);
	rt->cylinder->radius = ft_value(line[3], ',', 0, rt) / 2;
	rt->cylinder->height = ft_value(line[4], ',', 0, rt);
	rt->cylinder->color.x = ft_value(line[5], ',', 0, rt);
	rt->cylinder->color.y = ft_value(line[5], ',', 1, rt);
	rt->cylinder->color.z = ft_value(line[5], ',', 1, rt);
	if (cylinder_errorcheck(rt) == -1)
		return (-1);
	return (0);
}

int	parse_cylinder(char **line, t_rt *rt)
{
	int			size;
	t_cylinder	*temp;

	temp = rt->cylinder;
	while (rt->cylinder != NULL && rt->cylinder->next != NULL)
		rt->cylinder = rt->cylinder->next;
	if (rt->cylinder == NULL)
		rt->cylinder = new_cylinder();
	else
	{
		rt->cylinder->next = new_cylinder();
		rt->cylinder = rt->cylinder->next;
	}
	size = array_size(line);
	if (size != 6)
		return (-1);
	if (alpha_check(line) == -1)
		return (-1);
	if (set_cylinder(line, rt) == -1)
		return (-1);
	if (temp != NULL)
		rt->cylinder = temp;
	return (0);
}

void	free_split(char **line)
{
	int	i;

	i = 0;
	while (line[i] != NULL)
	{
		free (line[i]);
		i++;
	}
	free (line[i]);
}

int	parse_line(char *str, t_rt *rt)
{
	char	**line;

	line = ft_split(str, ' ');
	if (line != NULL && ft_strncmp(line[0], "A", 2) == 0)
		return (parse_ambient(line, rt));
	else if (line != NULL && ft_strncmp(line[0], "C", 2) == 0)
		return (parse_camera(line, rt));
	else if (line != NULL && ft_strncmp(line[0], "L", 2) == 0)
		return (parse_light(line, rt));
	else if (line != NULL && ft_strncmp(line[0], "sp", 3) == 0)
		return (parse_sphere(line, rt));
	else if (line != NULL && ft_strncmp(line[0], "pl", 3) == 0)
		return (parse_plane(line, rt));
	else if (line != NULL && ft_strncmp(line[0], "cy", 3) == 0)
		return (parse_cylinder(line, rt));
	else if (line != NULL && ft_strncmp(line[0], "\n", 2) != 0)
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

int	parse_set_rt(t_rt *rt, int fd)
{
	char	*str;

	str = get_next_line(fd);
	while (str != NULL)
	{
		str = fixed_line(str);
		if (ft_strlen(str) > 0 && parse_line(str, rt) == -1)
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
	int	fd;

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
