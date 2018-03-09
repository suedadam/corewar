/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:42:15 by asyed             #+#    #+#             */
/*   Updated: 2018/03/08 19:08:45 by asyed            ###   ########.fr       */
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
		if (!children->die_check)
		{
			if (children == g_taskmanager->processes)
			{
				printf("(%zu) Process = %d never called live\n", g_taskmanager->currCycle, children->plid);
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
	}
	else
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
		}
	}
}
