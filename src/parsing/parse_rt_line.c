#include "../../inc/MiniRt.h"

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
	free (line);
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

int	parse_line(char *str, t_rt *rt)
{
	char	**line;
	int		ret;

	line = ft_split(str, ' ');
	ret = 0;
	if (line != NULL && ft_strncmp(line[0], "A", 2) == 0)
		ret = parse_ambient(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "C", 2) == 0)
		ret = parse_camera(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "L", 2) == 0)
		ret = parse_light(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "sp", 3) == 0)
		ret = parse_sphere(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "pl", 3) == 0)
		ret = parse_plane(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "cy", 3) == 0)
		ret = parse_cylinder(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "pa", 3) == 0)
		ret = parse_paraboloid(line, rt);
	else if (line != NULL && ft_strncmp(line[0], "\n", 2) != 0)
		ret =  -1;
	free_split(line);
	return (ret);
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