/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 20:40:44 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, t_process *child)
{
	UNUSED(arena);
	printf(" %d", (short)(cmd_input->args[0]));
	if (child->carry == 1)
	{
		printf(" OK");
		child->pc = MEM_WARP(child->pc + (short)(cmd_input->args[0]) % IDX_MOD);
	}
	else
		printf(" FAILED");
	return (0);
}
