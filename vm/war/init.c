/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:42:15 by asyed             #+#    #+#             */
/*   Updated: 2018/03/11 23:27:56 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	remove_dead(void)
{
	int			player;
	t_process	*prevp;
	t_process	*children;

	player = 0;
	children = g_taskmanager->processes;
	while (children)
	{
		// printf("Checking child %d\n", children->pid);
		if (!children->die_check)
		{
			printf("Process %d hasn't lived for <whatever> cycles (CTD %d)\n", children->pid, g_taskmanager->c_to_die);
			// printf("PID: %d Hasn't lived!\n", children->pid);
			if (children == g_taskmanager->processes)
			{
				// printf("(%zu) Process = %d never called live\n", g_taskmanager->currCycle, children->plid);
				g_taskmanager->processes = children->next;
				free(children);
				prevp = NULL;
				children = g_taskmanager->processes;
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

static int	cleanup(void)
{
	int	p_left;

	if ((p_left = remove_dead()) <= 0)
	{
		printf("Player %d, has won!\n", g_taskmanager->lastlive);
		return (1);
	}
	if (g_taskmanager->lastnbrlive >= NBR_LIVE ||
		(g_taskmanager->c_checks + 1) == MAX_CHECKS)
	{
		g_taskmanager->c_to_die -= CYCLE_DELTA;
		g_taskmanager->lastnbrlive = 0;
		g_taskmanager->c_checks = 0;
		printf("Cycle to die is now %d\n", g_taskmanager->c_to_die);
	}
	g_taskmanager->c_checks++;
	g_taskmanager->c_diecycles = 0;
	return (0);
}

int			init_war(void *arena)
{
	int			p_left;
	t_process	*child;

	while (1)
	{
		if (!(child = g_taskmanager->processes))
			return (0);
		while (child)
		{
			run_operation(child->plid, arena, child);
			child = child->next;
		}
		if (!child)
		{
			if (!(g_taskmanager->c_diecycles % g_taskmanager->c_to_die)
				&& g_taskmanager->c_diecycles > 0)
			{
				if (cleanup())
					return (0);
			}
			(g_taskmanager->currCycle)++;
			(g_taskmanager->c_diecycles)++;
			printf("It is now cycle %zu\n", g_taskmanager->currCycle);
		}
	}
}
