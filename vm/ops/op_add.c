/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 07:27:08 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_add(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	printf("(%d) {ADD}\n", taskmanager->currCycle);
	child->regs[(cmd_input->args)[2]] = child->regs[(cmd_input->args)[0]] + child->regs[(cmd_input->args)[1]];
	if (child->regs[(cmd_input->args)[2]])
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
