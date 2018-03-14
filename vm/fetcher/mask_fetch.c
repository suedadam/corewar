/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mask_fetch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 07:41:59 by asyed             #+#    #+#             */
/*   Updated: 2018/03/13 17:31:25 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_reg(t_byte *arena, t_process *child, int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, child->pc + *size, sizeof(t_byte));
	*size += F_REG_SIZE;
}

void	handle_dir(t_byte *arena, t_process *child, int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, MEM_WARP(child->pc + *size),
		sizeof(int));
	*storage = ft_longswap(*storage);
	*size += F_DIR_SIZE;
}

void	handle_ind(t_byte *arena, t_process *child, int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, child->pc + *size, sizeof(short));
	*storage = ft_shortswap(*storage);
	*size += F_IND_SIZE;
}

int		fetch_decider(t_byte *arena, t_operation *cmd, t_process *child)
{
	t_byte	byte;

	byte = (cmd->encbyte << (2 * cmd->ac)) & 0xC0;
	if (byte == (t_byte)SHIFT_T_REG)
	{
		if ((g_op_tab[child->opcode - 1].encbyte[cmd->ac] & T_REG) != T_REG)
			return (-1);
		handle_reg(arena, child, &((cmd->args)[cmd->ac]), &cmd->size);
	}
	else if (byte == (t_byte)SHIFT_T_DIR && !g_op_tab[child->opcode - 1].trunc)
	{
		if ((g_op_tab[child->opcode - 1].encbyte[cmd->ac] & T_DIR) != T_DIR)
			return (-1);
		handle_dir(arena, child, &((cmd->args)[cmd->ac]), &cmd->size);
	}
	else if (byte == (t_byte)SHIFT_T_IND ||
		(byte == (t_byte)SHIFT_T_DIR && g_op_tab[child->opcode - 1].trunc))
	{
		if (((g_op_tab[child->opcode - 1].encbyte[cmd->ac] & T_IND) != T_IND) &&
			(g_op_tab[child->opcode - 1].encbyte[cmd->ac] & T_DIR) != T_DIR)
			return (-1);
		handle_ind(arena, child, &((cmd->args)[cmd->ac]), &cmd->size);
	}
	return (0);
}
