#include "libft.h"

int	ft_isspace(char s)
{
	if (s == ' ' || s == '\n' || s == '\t')
		return (1);
	if (s == '\v' || s == '\f' || s == '\r')
		return (1);
	return (0);
}