/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lldi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 01:25:19 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

char arg1[1024];
char arg2[1024];
char arg3[1024];

static int	lldi_reg(unsigned char *arena, t_process *child, t_andop *op_data)
{
	int	tmp;

	if (!op_data->argi)
	{
		op_data->val = child->regs[*(op_data->arg)];
		sprintf(arg1, "%d", child->regs[*(op_data->arg)]);
	}
	else if (op_data->argi == 1)
	{
		op_data->val += child->regs[*(op_data->arg)];
		sprintf(arg2, "%d", child->regs[*(op_data->arg)]);
		// tmp = 0;
		// copy_memory_fwd_off(&tmp, arena, child->pc + op_data->val % IDX_MOD,
		// 	sizeof(short));
		// op_data->val = ntohs(tmp);
	}
	else
	{
		op_data->dest = &(child->regs[*(op_data->arg)]);
		sprintf(arg3, "r%d", *(op_data->arg));
	}
	return (0);
}

/*
** We're using them as Indexes so they have to be direct...not indirect
** even tho truncation :/
*/

static int	lldi_dir(unsigned char *arena, t_process *child, t_andop *op_data)
{
	int	tmp;

	if (!op_data->argi)
	{
		op_data->val = (short)*(op_data->arg);
		sprintf(arg1, "%d", (short)*(op_data->arg));
	}
	else if (op_data->argi == 1)
	{
		op_data->val += (short)*(op_data->arg);
		sprintf(arg2, "%d", (short)*(op_data->arg));
		// tmp = 0;
		// copy_memory_fwd_off(&tmp, arena, MEM_WARP(child->pc + op_data->val % IDX_MOD),
		// 	sizeof(short));
		// op_data->val = ntohs(tmp);
	}
	return (0);
}

static int	lldi_decoder(void *arena, t_process *child, t_andop *op_data)
{
	unsigned char	byte;

	byte = ((op_data->encbyte << (2 * op_data->argi)) & 0xC0);
	if (byte == (unsigned char)SHIFT_T_REG)
		lldi_reg(arena, child, op_data);
	else if (byte == (unsigned char)SHIFT_T_IND ||
			byte == (unsigned char)SHIFT_T_DIR)
		lldi_dir(arena, child, op_data);
	return (0);
}

int			op_lldi(t_operation *cmd_input, void *arena,
					uint8_t plid, t_process *child)
{
	int				i;
	unsigned char	byte;
	t_andop			op_data;

	printf("ldi ");
	bzero(&op_data, sizeof(t_andop));
	op_data.encbyte = cmd_input->encbyte;
	byte = cmd_input->encbyte;
	i = 0;
	while (byte)
	{
		op_data.arg = &((cmd_input->args)[i]);
		op_data.argi = i;
		if (!lldi_decoder(arena, child, &op_data))
			i++;
		byte = byte << 2;
	}
	printf("%s %s %s\n       | -> load from %s + %s = %d (with pc and mod %lld)\n", arg1, arg2, arg3, arg1, arg2, op_data.val, MEM_WARP(child->pc + op_data.val));
	*(op_data.dest) = 0;
	copy_memory_fwd_off(op_data.dest, arena,
		MEM_WARP(child->pc + op_data.val), sizeof(int));
	*(op_data.dest) = ntohl(*(op_data.dest));
	if (*(op_data.dest))
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
