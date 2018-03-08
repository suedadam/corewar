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
		if (!(taskmanager->currCycle % taskmanager->c_to_die) && taskmanager->currCycle > 0)
		{
			if ((p_left = remove_dead()) <= 0)
			{
				printf("GAME OVER BITCHES player %d won!\n", taskmanager->lastlive);
				return (0);
			}
			else
			{
				printf("%d players left\n", p_left);
			}
		}
		if ((taskmanager->currCycle - taskmanager->lastnbrlive) == NBR_LIVE)
		{
			// printf("REDUCED!!!\n");
			// if (taskmanager->c_to_die - CYCLE_DELTA >= taskmanager->c_to_die)
			// 	taskmanager->c_to_die = 0;
			// else
			// 	taskmanager->c_to_die -= CYCLE_DELTA;
			// taskmanager->lastnbrlive = taskmanager->currCycle;
		}
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
			(taskmanager->currCycle)++;
			// if (taskmanager->currCycle > 2829) //1126
			if (taskmanager->currCycle > 3000) //1126
			{
				int j;
				unsigned char* byte_array = arena;

				j = 0;
				while (j < MEM_SIZE)
				// while (j < 1024)
				{
					printf("%02x ",(unsigned)byte_array[j]);
					j++;
				}
				exit(1);
			}
		}
	}
}
