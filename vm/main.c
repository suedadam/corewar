/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 14:49:32 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 20:39:45 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_taskmanager	*g_taskmanager;

void	*init(int champc, char **champv)
{
	void			*arena;
	int				i;
	t_taskmanager	*l_taskmanager;

	if (!(arena = init_arena()))
		return (NULL);
	if (!(l_taskmanager = ft_memalloc(sizeof(t_taskmanager))))
		return (NULL);
	g_taskmanager = l_taskmanager;
	g_taskmanager->c_to_die = CYCLE_TO_DIE;
	g_taskmanager->total_players = champc;
	g_taskmanager->lastlive = -1;
	g_taskmanager->lastnbrlive = 0;
	i = 0;
	while (i < champc)
	{
		if (i > MAX_PLAYERS ||
			read_champion(champv[i + 1], arena, -(i + 1)) == -1)
		{
			free(arena);
			return (NULL);
		}
		i++;
	}
	return (arena);
}

int		main(int argc, char *argv[])
{
	void	*arena;

	printf("whatever\n");
	if (argc < 2)
	{
		printf("No champions listed\n");
		return (-1);
	}
	if (flag_parse(&argc, &argv))
	{
		printf("Flag parsing error\n");
		return (-1);
	}
	if (!(arena = init(argc, argv)))
		return (-1);
	init_war(arena);
	return (0);
}
