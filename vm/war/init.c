/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:42:15 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 16:06:13 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	remove_dead(void)
{
	int			player;
	t_process	*prevp;
	t_process	*children;
	void		*tmp;

	player = 0;
	children = taskmanager->processes;
	while (children)
	{
		if (!children->die_check)
		{
			if (children == taskmanager->processes)
			{
				taskmanager->processes = children->next;
				free(children);
				prevp = NULL;
				children = taskmanager->processes;
			}
			else
			{
				prevp->next = children->next;
				free(children);
				children = prevp->next;
			}
		}
		else
		{
			player++;
			children->die_check = 0;
			prevp = children;
			children = children->next;
		}
	}
	return (player);
}

/*
** Itterate and run through ->processes (children/forks) 
** that are present.
*/

int	init_war(void *arena)
{
	//Lets start this BITCHESSS.'C:'
	int			i;
	int			p_left;
	t_process	*child;

	i = 0;
	while (1)
	{
		// if (!taskmanager->c_to_die)
		// {
		// 	printf("\nCycle %d done\n", taskmanager->currCycle);
		// 		int j;
		// 		unsigned char* byte_array = arena;

		// 		j = 0;
		// 		while (j < MEM_SIZE)
		// 		// while (j < 1024)
		// 		{
		// 			printf("%02x ",(unsigned)byte_array[j]);
		// 			j++;
		// 		}
		// 	exit(2);
		// }
		if (!(child = taskmanager->processes))
		{
			printf("Everyjuan is dead O.o\n");
			return (0);
		}
		while (child)
		{
			// printf("Running operation for player: %d\n", child->pID);
			run_operation(child->pID, arena, child);
			child = child->next;
		}
		if (!child)
		{
			if (!(taskmanager->c_diecycles % taskmanager->c_to_die) && taskmanager->c_diecycles > 0)
			// if (taskmanager->c_to_die <= 0 || (!(taskmanager->currCycle % taskmanager->c_to_die) && taskmanager->currCycle > 0))
			{
				if ((p_left = remove_dead()) <= 0)
				{
					printf("GAME OVER BITCHES player %d won!\n", taskmanager->lastlive);
					int j;
					unsigned char* byte_array = arena;

					j = 0;
					while (j < MEM_SIZE)
					// while (j < 1024)
					{
						printf("%02x ",(unsigned)byte_array[j]);
						j++;
					}
					return (0);
				}
				else
				{
					printf("%d players left\n", p_left);
				}
				if (taskmanager->currCycle == 21806)
				{
					printf("%d >= %d\n", taskmanager->lastnbrlive, NBR_LIVE);
					printf("penis\n");
					exit(1);
				}
				if (taskmanager->lastnbrlive >= NBR_LIVE || (taskmanager->c_checks + 1) == MAX_CHECKS)
				{
					printf("{GCCF} (%d;%d) %d -> %d\n", taskmanager->currCycle, taskmanager->c_diecycles, taskmanager->c_to_die, taskmanager->c_to_die - CYCLE_DELTA);
					taskmanager->c_to_die -= CYCLE_DELTA;
					taskmanager->lastnbrlive = 0;
					taskmanager->c_checks = 0;
				}
				else
					taskmanager->c_checks++;
				taskmanager->c_diecycles = 0;
			}
			// if (!(taskmanager->lastnbrlive % NBR_LIVE) && taskmanager->lastnbrlive > 0)
			(taskmanager->currCycle)++;
			(taskmanager->c_diecycles)++;
			// if (taskmanager->currCycle > 2829) //1126
			// if (taskmanager->currCycle > 60000) //1126
			// {
			// 	int j;
			// 	unsigned char* byte_array = arena;

			// 	j = 0;
			// 	while (j < MEM_SIZE)
			// 	// while (j < 1024)
			// 	{
			// 		printf("%02x ",(unsigned)byte_array[j]);
			// 		j++;
			// 	}
			// 	exit(1);
			// }
		}
	}
}
