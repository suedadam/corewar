/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/10 23:41:13 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_add(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	printf("add r%d r%d r%d\n", cmd_input->args[0], cmd_input->args[1], cmd_input->args[2]);
	child->regs[(cmd_input->args)[2]] =
		child->regs[(cmd_input->args)[0]] + child->regs[(cmd_input->args)[1]];
	if (child->regs[(cmd_input->args)[2]])
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
