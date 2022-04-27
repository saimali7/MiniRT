#include "../inc/MiniRt.h"
#include "../Libft/libft.h"

int	rt_atoi(const char *str, t_rt *rt)
{
	int				i;
	int				sign;
	unsigned int	res;

	i = 0;
	sign = 1;
	res = 0;
	if (str[i] == '\0')
		return (0);
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if ((str[i] != '\0') || (res > 2147483647 && sign == 1) || \
	(res > 2147483648 && sign == -1))
	{
		free_rt(&rt);
		exit(1);
	}
	return (res * sign);
}
static void	rt_pop_dot(char *str)
{
	int	i;

	i = ft_strchr(str, '.') - str;
	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
}

static double	rt_devide(int size)
{
	double	p;

	p = 1;
	while (size)
	{
		p *= 10;
		size--;
	}
	return (p);
}

double	rt_atod(const char *str, t_rt *rt)
{
	char	*tmp;
	int		dot;
	double	nb;

	if (!ft_strchr(str, '.'))
		return (rt_atoi(str, rt));
	tmp = ft_strdup(str);
	if (!tmp)
		return (0.0l);
	dot = ft_strchr(str, '.') - str;
	rt_pop_dot(tmp);
	nb = rt_atoi(tmp, rt);
	free(tmp);
	return (nb / rt_devide(ft_strlen(str) - 1 - dot));
}
