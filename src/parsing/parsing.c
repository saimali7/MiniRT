#include "../../inc/MiniRt.h"

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
