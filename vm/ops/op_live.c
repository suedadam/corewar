/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_live.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 05:29:10 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 13:00:30 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_live(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	int			p_input;
	t_process	*lostkid;

	p_input = cmd_input->args[0];
	child->die_check = 1;
	g_taskmanager->lastnbrlive++;
	if (p_input <= g_taskmanager->totalPlayers)
	{
		if (!(lostkid = g_taskmanager->processes))
		{
			printf("Player %d has won!\n", g_taskmanager->lastlive);
			exit(1);
		}
		while (lostkid)
		{
			if (lostkid->plid == p_input)
				break ;
			lostkid = lostkid->next;
		}
		if (lostkid)
			g_taskmanager->lastlive = p_input;
	}
	return (0);
}
