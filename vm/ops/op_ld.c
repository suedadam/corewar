/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_ld.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/07 04:27:41 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_ld(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	unsigned char	tmp;
	unsigned char	byte;
	int				val;

	printf("(%d) {LD}\n", taskmanager->currCycle);
	byte = cmd_input->encbyte;
	val = 0;
	if ((byte & 0xC0) == (unsigned char)SHIFT_T_DIR)
	{
		val = (cmd_input->args)[0];
		printf("(%d) {LD} UID: %d Value = %d\n", taskmanager->currCycle, child->randID, val);
	}
	else if ((byte & 0xC0) == (unsigned char)SHIFT_T_IND)
	{
		printf("(%d) {LD} UID: %d Offset = %d\n", taskmanager->currCycle, child->randID, (child->pc + (cmd_input->args)[0] % IDX_MOD));
		copy_memory_fwd_off(&val, arena, (child->pc + (cmd_input->args)[0] % IDX_MOD), F_IND_SIZE);
	}
	else
	{
		printf("Undefined\n");
		exit(1);
	}
	child->regs[(cmd_input->args)[1]] = val;
	if (!(child->regs[(cmd_input->args)[1]]))
		child->carry = 1;
	else
		child->carry = 0;
	printf("(%d) {LD} UID: %d Fetched val = %x val = %d\n", taskmanager->currCycle, child->randID, child->regs[(cmd_input->args)[1]], (cmd_input->args)[0]);
	return (0);
}