/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_ld.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 15:31:24 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_ld(t_operation *cmd_input, void *arena, t_process *child)
{
	t_byte	byte;
	int32_t	val;

	byte = cmd_input->encbyte;
	val = 0;
	if ((byte & 0xC0) == (t_byte)SHIFT_T_DIR)
	{
		val = (cmd_input->args)[0] % IDX_MOD;
		child->regs[(cmd_input->args)[1]] = val;
	}
	else if ((byte & 0xC0) == (t_byte)SHIFT_T_IND)
	{
		copy_memory_fwd_off(&val, arena,
			MEM_WARP((child->pc + (short)(cmd_input->args)[0] % IDX_MOD)),
			F_IND_SIZE);
		val = ft_shortswap(val);
		child->regs[(cmd_input->args)[1]] = (short)val;
	}
	if (!(child->regs[(cmd_input->args)[1]]))
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
