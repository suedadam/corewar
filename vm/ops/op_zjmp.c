/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/08 20:00:54 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	if (child->carry == 1)
	{
		printf("(%zu) ZJMP - %lld -> %lld\n", g_taskmanager->currCycle, child->pc, (child->pc +
			(short)((cmd_input->args)[0]) + MEM_SIZE) % MEM_SIZE);
		child->pc = (child->pc +
			(short)((cmd_input->args)[0]) + MEM_SIZE) % MEM_SIZE;
	}
	return (0);
}
