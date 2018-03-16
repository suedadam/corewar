/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mask_fetch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 07:41:59 by asyed             #+#    #+#             */
/*   Updated: 2018/03/15 21:14:42 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		handle_reg(t_byte *arena, t_process *child, int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, child->pc + *size, sizeof(t_byte));
	if (*storage > REG_NUMBER || *storage < 0)
		return (-1);
	*size += F_REG_SIZE;
	return (0);
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

#define NOT_OP(x) ((g_op_tab[child->opcode - 1].encbyte[cmd->ac] & x) != x)
#define HANDLER_ARGS arena, child, &((cmd->args)[cmd->ac]), &cmd->size

int		fetch_decider(t_byte *arena, t_operation *cmd, t_process *child)
{
	t_byte	byte;

	byte = (cmd->encbyte << (2 * cmd->ac)) & 0xC0;
	if (byte == (t_byte)SHIFT_T_REG)
	{
		if (NOT_OP(T_REG) || handle_reg(HANDLER_ARGS))
			return (-1);
	}
	else if (byte == (t_byte)SHIFT_T_DIR && !g_op_tab[child->opcode - 1].trunc)
	{
		if (NOT_OP(T_DIR))
			return (-1);
		handle_dir(HANDLER_ARGS);
	}
	else if (byte == (t_byte)SHIFT_T_IND
		|| (byte == (t_byte)SHIFT_T_DIR && g_op_tab[child->opcode - 1].trunc))
	{
		if (NOT_OP(T_IND) && NOT_OP(T_DIR))
			return (-1);
		handle_ind(HANDLER_ARGS);
	}
	else
		return (-1);
	return (0);
}
