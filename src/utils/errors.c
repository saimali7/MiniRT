/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anifanto <anifanto@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 16:46:30 by anifanto          #+#    #+#             */
/*   Updated: 2022/07/04 18:49:50 by anifanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/MiniRt.h"

void	error_exit(int code, char *str, t_rt *rt)
{
	int	i;

	i = 0;
	if (code == -1)
		i = write(STDERR_FILENO, str, ft_strlen(str));
	if (rt)
		free_all(rt);
	exit(code);
}
