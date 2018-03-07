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

	byte = cmd_input->encbyte;
	val = 0;
	if ((byte & 0xC0) == (unsigned char)SHIFT_T_DIR)
		val = (cmd_input->args)[0];
	else if ((byte & 0xC0) == (unsigned char)SHIFT_T_IND)
	{
		//copy_memory_fwd_off(void *dest, unsigned char *arena, t_process *child, size_t size, int offset)
		copy_memory_fwd_off(&val, arena, child, F_IND_SIZE, (cmd_input->args)[0] % IDX_MOD);
	}
	else
	{
		printf("Undefined\n");
		exit(1);
	}
	child->regs[(cmd_input->args)[1]] = val;
	child->carry = 1;
	return (0);
}