/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_live.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 05:29:10 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 10:24:09 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_live(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	int			p_input;
	t_process	*lostkid;

	p_input = cmd_input->args[0];
	printf("Player = %d\n", p_input);
	child->die_check = 1;
	if (p_input <= taskmanager->totalPlayers)
	{
		if (!(lostkid = taskmanager->processes))
		{
			printf("GAME OVER MOTHER FUCKA\n");
			exit(1);
		}
		while (lostkid)
		{
			if (lostkid->pID == p_input)
				break ;
			lostkid = lostkid->next;
		}
		if (!lostkid)
			printf("Player is dead\n");
		else
			taskmanager->lastlive = p_input;
		// if (!((taskmanager->players)[p_input]))
			// printf("Player is dead!\n");
		// else
			// taskmanager->lastlive = p_input;
	}
	// exit(1);
	return (0);
}
