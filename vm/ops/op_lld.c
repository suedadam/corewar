/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lld.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 13:00:26 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_lld(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	unsigned char	byte;
	int				val;
	int				test;

	byte = cmd_input->encbyte;
	val = 0;
	if ((byte & 0xC0) == (unsigned char)SHIFT_T_DIR)
		val = (cmd_input->args)[0];
	else if ((byte & 0xC0) == (unsigned char)SHIFT_T_IND)
	{
		copy_memory_fwd_off(&val, arena,
				MEM_WARP((child->pc + (short)(cmd_input->args)[0])), F_IND_SIZE);
		val = ft_shortswap(val);
	}
	child->regs[(cmd_input->args)[1]] = val;
	if (!(child->regs[(cmd_input->args)[1]]))
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
