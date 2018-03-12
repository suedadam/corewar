/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lfork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 02:20:46 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_lfork(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	static int pid = 2; //Remove me.
	t_process	*new;

	printf("lfork %d (%lld)\n", (short)cmd_input->args[0], MEM_WARP(child->pc + (short)cmd_input->args[0]));
	if (!(new = ft_memalloc(sizeof(t_process))))
		exit(1);
	memcpy(new, child, sizeof(t_process));
	new->pc = MEM_WARP(child->pc + (short)cmd_input->args[0]);
	// new->pc = (child->pc + (short)(cmd_input->args[0]) + MEM_SIZE) % MEM_SIZE;
	new->plid = child->plid;
	new->pid = pid++;
	new->next = g_taskmanager->processes;
	g_taskmanager->processes = new;
	raincheck(arena, new);
	return (0);
}
