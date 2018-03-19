/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 20:51:59 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_add(t_operation *cmd_input, void *arena, t_process *child)
{
	UNUSED(arena);
	printf(" r%d r%d r%d", (cmd_input->args[0]), cmd_input->args[1], cmd_input->args[2]);
	child->regs[(cmd_input->args)[2]] =
		child->regs[(cmd_input->args)[0]] + child->regs[(cmd_input->args)[1]];
	if (child->regs[(cmd_input->args)[2]])
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
