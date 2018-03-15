/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lfork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/15 14:13:34 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_lfork(t_operation *cmd_input, void *arena, t_process *child)
{
	t_process	*new;

	if (!(new = ft_memalloc(sizeof(t_process))))
		exit(1);
	ft_memcpy(new, child, sizeof(t_process));
	new->pc = MEM_WARP(child->pc + (short)cmd_input->args[0]);
	new->plid = child->plid;
	new->next = g_taskmanager->processes;
	g_taskmanager->processes = new;
	raincheck(arena, new);
	return (0);
}
