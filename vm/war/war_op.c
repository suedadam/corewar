/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:51:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/14 19:59:55 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	calc_enc_size(t_process *child, t_byte byte)
{
	int		size;
	int		argc;
	t_byte	tmp;

	argc = g_op_tab[child->opcode - 1].argc;
	size = 0;
	while (argc--)
	{
		tmp = byte & 0xC0;
		if (tmp == (t_byte)SHIFT_T_REG)
			size += F_REG_SIZE;
		else if (tmp == (t_byte)SHIFT_T_DIR)
		{
			if (g_op_tab[child->opcode - 1].trunc)
				size += F_IND_SIZE;
			else
				size += F_DIR_SIZE;
		}
		else if (tmp == (t_byte)SHIFT_T_IND)
			size += F_IND_SIZE;
		byte = byte << 2;
	}
	return (size);
}

static int	fetch_input(t_byte *arena, t_operation *cmd,
	t_process *child)
{
	int	size;

	size = 1;
	if (g_op_tab[child->opcode - 1].encbyte[0] == T_DIR)
	{
		if (g_op_tab[child->opcode - 1].trunc)
			handle_ind(arena, child, &(cmd->args[0]), &size);
		else
			handle_dir(arena, child, &(cmd->args[0]), &size);
	}
	else
		return (-1);
	return (size);
}

static int	decode_acb(t_byte *arena, t_operation *cmd, t_process *child)
{
	t_byte	byte;

	cmd->encbyte = 0;
	copy_memory_fwd_off(&cmd->encbyte, arena, child->pc + 1, 1);
	byte = cmd->encbyte;
	cmd->ac = 0;
	cmd->size = 2;
	while (cmd->ac < g_op_tab[child->opcode - 1].argc && byte)
	{
		if (fetch_decider(arena, cmd, child))
			break;
		cmd->ac++;
		byte = byte << 2;
	}
	if (cmd->ac != g_op_tab[child->opcode - 1].argc)
	{
		invalid_acb(arena, child, 2 + calc_enc_size(child, cmd->encbyte));
		return (-1);
	}
	return (cmd->size);
}

void		raincheck(void *arena, t_process *child)
{
	int	opcode;

	opcode = 0;
	copy_memory_fwd_off(&opcode, arena, child->pc, sizeof(t_byte));
	if ((opcode > 16 || opcode < 1))
	{
		invalid_opcode(child);
		return ;
	}
	child->opcode = opcode;
	child->run_op = g_taskmanager->curr_cycle +
					g_op_tab[(opcode - 1)].waitcycles;
}

int			run_operation(t_byte *arena, t_process *child)
{
	t_operation		cmd;
	int				size;

	if (!(child->run_op) || !(child->opcode))
		raincheck(arena, child);
	else if (child->run_op == g_taskmanager->curr_cycle)
	{
		if (g_op_tab[child->opcode - 1].encbool)
		{
			if ((size = decode_acb(arena, &cmd, child)) == -1)
				return (-1);
		}
		else if ((size = fetch_input(arena, &cmd, child)) == -1)
			return (-1);
		if (g_opdispatch[child->opcode - 1].func(&cmd, arena,
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
