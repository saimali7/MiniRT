#include "../inc/MiniRt.h"

void error_exit(int code, char *str)
{
    if (code == -1)
        write (STDERR_FILENO, str, ft_strlen(str));
    //free_all;
    exit(code);
}