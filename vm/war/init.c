/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:42:15 by asyed             #+#    #+#             */
/*   Updated: 2018/03/14 20:32:25 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	remove_dead(void)
{
	t_process	**curr;
	t_process	*next;
	int			count;

	count = 0;
	curr = &g_taskmanager->processes;
	while (*curr)
	{
		if (!(*curr)->die_check)
		{
			next = (*curr)->next;
			free(*curr);
			*curr = next;
		}
		else
		{
			++count;
			(*curr)->die_check = 0;
			curr = &(*curr)->next;
		}
	}
	return (count);
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
	g_taskmanager->c_checks++;
	g_taskmanager->c_diecycles = 0;
	return (0);
}

void		init_war(void *arena)
{
	t_process	*child;
	int			*dump;

	dump = fetch_result('D', 1);
	while (1)
	{
		if (!(child = g_taskmanager->processes))
			return ;
		while (child)
		{
			run_operation(arena, child);
			child = child->next;
		}
		if (!child)
		{
			if (g_taskmanager->c_diecycles >= g_taskmanager->c_to_die &&
				cleanup())
				return ;
			g_taskmanager->curr_cycle++;
			g_taskmanager->c_diecycles++;
		}
		if (dump && g_taskmanager->curr_cycle > (unsigned long)*dump)
			return (dump_memory(arena));
	}
}
