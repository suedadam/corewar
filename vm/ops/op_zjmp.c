/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/09 03:44:33 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	if (child->carry == 1)
	{
		printf("(%zu) {ZJMP} %d = %lld\n", g_taskmanager->currCycle,
			(short)(cmd_input->args[0]), 
			MEM_WARP(child->pc + (short)(cmd_input->args[0]))
			);
		child->pc = MEM_WARP(child->pc + (short)(cmd_input->args[0]));
	}
	else
		printf("(%zu) {ZJMP} Carry not set? %d\n", g_taskmanager->currCycle, child->carry);
	return (0);
}
