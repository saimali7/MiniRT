/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anifanto <anifanto@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 16:24:51 by anifanto          #+#    #+#             */
/*   Updated: 2022/07/05 10:16:42 by anifanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/MiniRt_bonus.h"

t_rt	*init_rt(void)
{
	t_rt	*ptr;

	ptr = ft_calloc(sizeof(t_rt), 1);
	if (!ptr)
		error_exit(-1, ERR_MEM_AL, NULL);
	ptr->plane = NULL;
	ptr->sphere = NULL;
	ptr->cylndr = NULL;
	ptr->parab = NULL;
	ptr->display = NULL;
	return (ptr);
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
			free_all(rt);
			error_exit(-1, "Error: Invalid argument file\n", NULL);
		}
		init_window(rt->display, rt);
	}
	else
		error_exit(-1, "Error: Usage: ./miniRT path_of_scene.rt\n", NULL);
	free_all(rt);
	return (0);
}
