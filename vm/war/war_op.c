/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:51:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 12:08:34 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

unsigned char *g_arena;

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
	// printf("(%zu) ARG: %d - %d\n", g_taskmanager->currCycle, 0, (cmd_input->args)[0]);
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
	//This check is broken/causing issues.
	if ((cmd_input->encbyte >> (8 - (2 * g_op_tab[child->opcode - 1].argc))) == 00)
	{
		// printf("(%zu) {Invalid} Test here. L 81 %d vs %d\n", g_taskmanager->currCycle, j, g_op_tab[child->opcode - 1].argc);
		invalid_acb(child, 2);
		return (-1);
	}
	else if (j != g_op_tab[child->opcode - 1].argc)
	{
		// printf("(%zu) {Invalid} Test here. L 86 %d vs %d\n", g_taskmanager->currCycle, j, g_op_tab[child->opcode - 1].argc);
		invalid_acb(child, 2 + calc_enc_size(child, cmd_input->encbyte));
		return (-1);
	}
	return (size);
}

void		raincheck(void *arena, t_process *child)
{
	int	opcode;

	opcode = 0;
	copy_memory_fwd_off(&opcode, arena, child->pc, sizeof(unsigned char));
	if ((opcode > 15 || opcode <= 0))
	{
		// printf("(%zu) Invalid opcode (%02x)\n", g_taskmanager->currCycle, opcode);
		invalid_opcode(child);
		return ;
	}
	child->opcode = opcode;
	child->run_op = g_taskmanager->currCycle +
					g_op_tab[(opcode - 1)].waitcycles;
}

int			run_operation(int plid, unsigned char *arena, t_process *child)
{
	unsigned char	*ophex;
	t_operation		cmd_input;
	int				size;

	g_arena = arena;
	if (!(child->run_op) || !(child->opcode))
	{
		// printf("(%zu) what the hell?\n", g_taskmanager->currCycle);
		raincheck(arena, child);
	}
	else if (child->run_op == g_taskmanager->currCycle)
	{
		if (g_op_tab[child->opcode - 1].encbool)
		{
			if ((size = decode_acb(arena, &cmd_input, child)) == -1)
			{
				// printf("(%zu) Invalid ACB\n", g_taskmanager->currCycle);
				return (-1);
			}
		}
		else if ((size = fetch_input(arena, &cmd_input, child)) == -1)
		{
			// printf("(%zu) Invalid fetch_input\n", g_taskmanager->currCycle);
			return (-1);
		}
		// printf("(%zu) {%s} %d %d %d\n", g_taskmanager->currCycle, g_op_tab[child->opcode - 1].op_name, (short)cmd_input.args[0], (short)cmd_input.args[1], cmd_input.args[2]);
		printf("P%5d | ", child->pid);
		if (g_opdispatch[child->opcode - 1].func(&cmd_input, arena,
											plid, child) == -1)
		{
			printf("(%zu) Invalid Function return %d\n", g_taskmanager->currCycle, child->opcode);
			return (-1);
		}
		if (child->opcode != 9 || (child->opcode == 9 && !child->carry))
		{
			printf("ADV %d (0x%04llx -> 0x%04llx) ", size, child->pc, MEM_WARP(child->pc + size));
			// unsigned char *b_arr = arena + child->pc;
			int start = child->pc;
			int end = size;
			while (end--)
			{
				printf("%02x ", *(unsigned char *)(arena + MEM_WARP(start)));
				start++;
			}
			printf("\n");
			// child->pc = ((child->pc + size) + MEM_SIZE) % MEM_SIZE;
			child->pc = MEM_WARP(child->pc + size);
		}
		child->run_op = 0;
		child->opcode = 0;
		raincheck(arena, child);
		// printf("(%zu) {%s}\n", g_taskmanager->currCycle, g_op_tab[child->opcode - 1].op_name);
	}
	t_process *childrenlol = g_taskmanager->processes;
	// while (childrenlol)
	// {
	// 	printf("(%zu) ID: %d (%02x)PC: %lld(%02x) CachedOP: %d Wait: %zu\n", g_taskmanager->currCycle, childrenlol->plid, *(unsigned char *)(arena + MEM_WARP(childrenlol->pc - 1)), childrenlol->pc, *(unsigned char *)(arena + childrenlol->pc), childrenlol->opcode, childrenlol->run_op);
	// 	// printf("----= Registers =---\n");
	// 	// int j;
	// 	// j = 1;
	// 	// while (j < 16)
	// 	// {
	// 	// 	printf("(%zu) R%d = %d\n", g_taskmanager->currCycle, j, childrenlol->regs[j]);
	// 	// 	j++;
	// 	// }
	// 	// printf("----= EO-Registers =---\n");
	// 	childrenlol = childrenlol->next;
	// }
	// printf("\n======\n");
	// if (g_taskmanager->currCycle == 18000)
	// {
		// printf("==========\n (%zu)", g_taskmanager->currCycle);
		// int j;
		// unsigned char* byte_array = arena;

		// j = 0;
		// while (j < MEM_SIZE)
		// {
		// 	printf("%02x ",(unsigned)byte_array[j]);
		// 	j++;
		// }
		// printf("==========\n");		
		// exit(1);
	// }

	return (0);
}
