/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mask_fetch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 07:41:59 by asyed             #+#    #+#             */
/*   Updated: 2018/03/08 22:03:15 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_reg(unsigned char *arena, t_process *child, 
						int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, child->pc + *size,
		sizeof(unsigned char));
	printf("(%zu) {REG} %d\n", g_taskmanager->currCycle, *storage);
	*size += F_REG_SIZE;
}

void	handle_dir(unsigned char *arena, t_process *child, 
						int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, MEM_WARP(child->pc + *size),
		sizeof(int));
	*storage = ntohl(*storage);
	printf("(%zu) {H_DIR} %lld + %d = %d\n", g_taskmanager->currCycle, child->pc, *size, *storage);
	*size += F_DIR_SIZE;
}

void	handle_ind(unsigned char *arena, t_process *child, 
						int32_t *storage, int *size)
{
	*storage = 0;
	copy_memory_fwd_off(storage, arena, child->pc + *size,
		sizeof(short));
	*storage = ntohs(*storage);
	printf("(%zu) {H_IND} %lld + %d = %d\n", g_taskmanager->currCycle, child->pc, *size, *storage);
	*size += F_IND_SIZE;
}

int			fetch_decider(unsigned char *arena, t_operation *cmd_input,
	t_process *child, int j, int *size)
{
	unsigned char	byte;

	byte = (cmd_input->encbyte << (2 * j)) & 0xC0;
	if (byte == (unsigned char)SHIFT_T_REG)
	{
		if ((g_op_tab[child->opcode - 1].encbyte[j] & T_REG) != T_REG)
		{
			printf("(%zu) L54\n", g_taskmanager->currCycle);
			invalid_opcode(child);
			return (-1);
		}
		handle_reg(arena, child, &((cmd_input->args)[j]), size);
	}
	else if (byte == (unsigned char)SHIFT_T_DIR && !g_op_tab[child->opcode - 1].trunc)
	{
		if ((g_op_tab[child->opcode - 1].encbyte[j] & T_DIR) != T_DIR)
		{
			printf("(%zu) L64\n", g_taskmanager->currCycle);
			invalid_opcode(child);
			return (-1);
		}
		handle_dir(arena, child, &((cmd_input->args)[j]), size);
	}
	else if (byte == (unsigned char)SHIFT_T_IND || 
		(byte == (unsigned char)SHIFT_T_DIR && g_op_tab[child->opcode - 1].trunc))
	{
		if (((g_op_tab[child->opcode - 1].encbyte[j] & T_IND) != T_IND) &&
			(g_op_tab[child->opcode - 1].encbyte[j] & T_DIR) != T_DIR)
		{
			printf("(%zu) L76\n", g_taskmanager->currCycle);
			invalid_opcode(child);
			return (-1);
		}
		handle_ind(arena, child, &((cmd_input->args)[j]), size);
	}
	printf("(%zu) ARG: %d - %d\n", g_taskmanager->currCycle, j, (cmd_input->args)[j]);
	return (0);
}
