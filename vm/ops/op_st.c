/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_st.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 02:18:17 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

char arg1[1024];
char arg2[1024];
char arg3[1024];

static int	st_reg(t_process *child, t_andop *op_data)
{
	if (!op_data->argi)
	{
		op_data->val = child->regs[*(op_data->arg)];
		// op_data->val = &(child->regs[*(op_data->arg)]);
		sprintf(arg1, "r%d", *(op_data->arg));
	}
	else if (op_data->argi == 1)
	{
		op_data->dest = &child->regs[*(op_data->arg)];
		// op_data->val = child->regs[*(op_data->arg)];
		sprintf(arg2, "%d", child->regs[*(op_data->arg)]);
	}
	// printf("(%zu) {STI_REG} op_data->val = %d REg: %d\n", g_taskmanager->currCycle, op_data->val, child->regs[*(op_data->arg)]);
	return (0);
}

static int	st_decider(void *arena, t_process *child, t_andop *op_data)
{
	unsigned char	byte;

	byte = (op_data->encbyte << (2 * op_data->argi) & 0xC0);
	if (byte == (unsigned char)SHIFT_T_REG)
	{
		// printf("r%d ", *(op_data->arg));
		st_reg(child, op_data);
	}
	else
		op_data->dest = op_data->arg;
	// else (byte == (unsigned char)SHIFT_T_IND)
	// {
	// 	// printf("%d ", *(op_data->arg));
	// 	st_ind(arena, child, op_data);
	// }
	return (0);
}

int			op_st(t_operation *cmd_input, void *arena,
				uint8_t plid, t_process *child)
{
	int				i;
	int				byteswap;
	unsigned char	byte;
	t_andop			op_data;

	printf("st ");
	bzero(&op_data, sizeof(t_andop));
	op_data.encbyte = cmd_input->encbyte;
	byte = cmd_input->encbyte;
	i = 0;
	while (byte)
	{
		op_data.arg = &((cmd_input->args)[i]);
		op_data.argi = i;
		if (!st_decider(arena, child, &op_data))
			i++;
		byte = byte << 2;
	}
	printf("%s %s %s\n       | -> store to %s + %s = %d (with pc and mod %lld)\n", arg1, arg2, arg3, arg2, arg3, op_data.val, child->pc + op_data.val % IDX_MOD);
	// byteswap = htonl(*(op_data.dest));
	byteswap = htonl(op_data.val);
	// printf("(%zu) {STI_WRITE} Offset: %lld Val : %d\n", g_taskmanager->currCycle, child->pc + op_data.val % IDX_MOD, op_data.val);
	write_memory(arena, (unsigned char *)&byteswap,
				MEM_WARP(child->pc + *op_data.dest), REG_SIZE);
	return (0);
}
