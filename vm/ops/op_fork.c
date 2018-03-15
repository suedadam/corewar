/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_fork.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/15 00:46:21 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_fork(t_operation *cmd_input, void *arena, t_process *child)
{
	t_process	*new;
	static int 	pid = 4;

	if (!(new = ft_memalloc(sizeof(t_process))))
		exit(1);
	memcpy(new, child, sizeof(t_process));
	new->pc = MEM_WARP(child->pc + (short)cmd_input->args[0] % IDX_MOD);
	new->plid = child->plid;
	new->pid = pid++;
	new->next = g_taskmanager->processes;
	g_taskmanager->processes = new;
	raincheck(arena, new);
	return (0);
}
