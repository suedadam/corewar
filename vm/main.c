/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 14:49:32 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 13:05:46 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_taskmanager	*taskmanager;

void	*init(int champc, char **champv)
{
	void			*arena;
	int				i;
	t_taskmanager	*l_taskmanager;

	taskmanager = l_taskmanager;
	if (!(arena = init_arena()))
		return (NULL);
	if (!(taskmanager = ft_memalloc(sizeof(t_taskmanager))))
		return (NULL);
	taskmanager->c_to_die = CYCLE_TO_DIE;
	taskmanager->totalPlayers = champc;
	taskmanager->lastlive = -1;
	taskmanager->lastnbrlive = 0;
	i = 0;
	while (i < champc)
	{
		printf("[ID: %d] Champ = \"%s\"\n", i, champv[champc - i]);
		if (read_champion(champv[champc - i], arena, i) == -1)
		{
			printf("wow fail\n");
			free(arena);
			return (NULL);
		}
		else
			printf("Read champion \"%s\" into memory\n", champv[champc - i]);
		printf("[ID: %d] loaded into memory!\n", i);
		i++;
	}
	printf("Printing arena: \n");
	int j;
	unsigned char* byte_array = arena;

	j = 0;
	while (j < MEM_SIZE)
	{
		printf("%02X",(unsigned)byte_array[j]);
		j++;
	}
	// exit(1);
	return (arena);
}

int	main(int argc, char *argv[])
{
	void	*arena;

	//Make a library parse flags func.
	if (argc < 2)
	{
		printf("No champions listed\n");
		return (-1);
	}
	if (!(arena = init(argc - 1, argv)))
	{
		printf("Failed to read.\n");
		return (-1);
	}
	init_war(arena);
	return (0);
}
