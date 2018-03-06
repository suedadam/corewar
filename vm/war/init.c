/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:42:15 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 07:18:49 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	remove_dead(void)
{
	int			player;
	int			left;
	int			p_left;
	t_process	*children;
	void		*tmp;

	player = 0;
	left = 0;
	while ((taskmanager->players)[player])
	{
		p_left = 0;
		children = (taskmanager->players)[player]->processes;
		while (children)
		{
			if (!children->die_check)
			{
				tmp = children;
				children = children->next;
				free(tmp);
				if ((taskmanager->players)[player]->processes == tmp)
					(taskmanager->players)[player]->processes = children;
			}
			else
			{
				p_left++;
				children->die_check = 0;
				children = children->next;
			}
		}
		if (p_left)
			left++;
		else
		{
			free((taskmanager->players)[player]);
			(taskmanager->players)[player] = NULL;
		}
		player++;
	}
	return (left);
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
			printf("REDUCED!!!\n");
			if (taskmanager->c_to_die - CYCLE_DELTA >= taskmanager->c_to_die)
				taskmanager->c_to_die = 0;
			else
				taskmanager->c_to_die -= CYCLE_DELTA;
			taskmanager->lastnbrlive = taskmanager->currCycle;
		}
		if ((taskmanager->players)[i] && (taskmanager->players)[i]->processes)
		{
			child = (taskmanager->players)[i]->processes;
			while (child)
			{
				run_operation(i, arena, child);
				child = child->next;
			}
			// printf("playerID = %d\n", (taskmanager->players)[i]->pID);
			i++;
		}
		else
		{
			(taskmanager->currCycle)++;
			i = 0;
		}
	}
}
