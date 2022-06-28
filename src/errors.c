#include "../inc/MiniRt.h"

void error_exit(int code, char *str)
{
	int i;

	i  = 0;
	if (code == -1)
		i =  write(STDERR_FILENO, str, ft_strlen(str));
	exit(code);
}