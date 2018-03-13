/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:51:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/13 16:36:51 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	calc_enc_size(t_process *child, unsigned char byte)
{
	int				size;
	int				argc;
	unsigned char	tmp;

	argc = g_op_tab[child->opcode - 1].argc;
	size = 0;
	while (argc--)
	{
		tmp = byte & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_REG)
			size += F_REG_SIZE;
		else if (tmp == (unsigned char)SHIFT_T_DIR)
		{
			if (g_op_tab[child->opcode - 1].trunc)
				size += F_IND_SIZE;
			else
				size += F_DIR_SIZE;
		}
		else if (tmp == (unsigned char)SHIFT_T_IND)
			size += F_IND_SIZE;
		byte = byte << 2;
	}
	return (size);
}

static int	fetch_input(unsigned char *arena, t_operation *cmd_input,
						t_process *child)
{
	int	size;

	size = 1;
	if (g_op_tab[child->opcode - 1].encbyte[0] == T_DIR)
	{
		if (g_op_tab[child->opcode - 1].trunc)
			handle_ind(arena, child, &(cmd_input->args[0]), &size);
		else
			handle_dir(arena, child, &(cmd_input->args[0]), &size);
	}
	else
		return (-1);
	return (size);
}

static int	decode_acb(unsigned char *arena, t_operation *cmd_input,
						t_process *child)
{
	int				j;
	int				size;
	unsigned char	byte;

	cmd_input->encbyte = 0;
	copy_memory_fwd_off(&cmd_input->encbyte, arena, child->pc + 1, 1);
	byte = cmd_input->encbyte;
	j = 0;
	size = 2;
	while (byte)
	{
		if (!fetch_decider(arena, cmd_input, child, j, &size))
			j++;
		byte = byte << 2;
	}
	if ((cmd_input->encbyte >> (8 - (2 * g_op_tab[child->opcode - 1].argc))) == 00)
	{
		invalid_acb(arena, child, 2);
		return (-1);
	}
	else if (j != g_op_tab[child->opcode - 1].argc)
	{
		invalid_acb(arena, child, 2 + calc_enc_size(child, cmd_input->encbyte));
		return (-1);
	}
	return (size);
}

void		raincheck(void *arena, t_process *child)
{
	int	opcode;

	opcode = 0;
	copy_memory_fwd_off(&opcode, arena, child->pc, sizeof(unsigned char));
	if ((opcode > 16 || opcode < 1))
	{
		invalid_opcode(child);
		return ;
	}
	child->opcode = opcode;
	child->run_op = g_taskmanager->currCycle +
					g_op_tab[(opcode - 1)].waitcycles;
}

int			run_operation(unsigned char *arena, t_process *child)
{
	t_operation		cmd_input;
	int				size;

	if (!(child->run_op) || !(child->opcode))
		raincheck(arena, child);
	else if (child->run_op == g_taskmanager->currCycle)
	{
		if (g_op_tab[child->opcode - 1].encbool)
		{
			if ((size = decode_acb(arena, &cmd_input, child)) == -1)
				return (-1);
		}
		else if ((size = fetch_input(arena, &cmd_input, child)) == -1)
			return (-1);
		if (g_opdispatch[child->opcode - 1].func(&cmd_input, arena,
			child) == -1)
			return (-1);
		if (child->opcode != 9 || (child->opcode == 9 && !child->carry))
			child->pc = MEM_WARP(child->pc + size);
		child->run_op = 0;
		child->opcode = 0;
		raincheck(arena, child);
	}
	return (0);
}
