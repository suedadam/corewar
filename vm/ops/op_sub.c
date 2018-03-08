/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_sub.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 07:25:46 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_sub(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	child->regs[(cmd_input->args)[2]] = child->regs[(cmd_input->args)[0]] - child->regs[(cmd_input->args)[1]];
	if (child->regs[(cmd_input->args)[2]])
		child->carry = 0;
	else
		child->carry = 1;
	printf("%d = %d - %d (%d)\n", child->regs[(cmd_input->args)[2]], child->regs[(cmd_input->args)[0]], child->regs[(cmd_input->args)[1]], child->carry);
	return (0);
}
