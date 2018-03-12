/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_xor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 02:18:36 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	xor_reg(t_process *child, t_andop *op_data)
{
	if (!op_data->argi)
		op_data->val = child->regs[*(op_data->arg)];
	else if (op_data->argi == 1)
		op_data->val = op_data->val ^ child->regs[*(op_data->arg)];
	else
		op_data->dest = &(child->regs[*(op_data->arg)]);
	return (0);
}

static int	xor_ind(unsigned char *arena, t_process *child, t_andop *op_data)
{
	int	tmp;

	if (!op_data->argi)
	{
		tmp = 0;
		copy_memory_fwd_off(&tmp, arena,
				child->pc + *op_data->arg % IDX_MOD, sizeof(short));
		op_data->val = ntohs(tmp);
	}
	else if (op_data->argi == 1)
	{
		tmp = 0;
		copy_memory_fwd_off(&tmp, arena,
				child->pc + *op_data->arg % IDX_MOD, sizeof(short));
		op_data->val = op_data->val ^ ntohs(tmp);
	}
	return (0);
}

static int	xor_dir(t_process *child, t_andop *op_data)
{
	if (!op_data->argi)
		op_data->val = *(op_data->arg);
	else if (op_data->argi == 1)
		op_data->val = op_data->val ^ *(op_data->arg);
	return (0);
}

static int	xor_decider(void *arena, t_process *child, t_andop *op_data)
{
	unsigned char	byte;

	byte = (op_data->encbyte << (2 * op_data->argi) & 0xC0);
	if (byte == (unsigned char)SHIFT_T_REG)
		xor_reg(child, op_data);
	else if (byte == (unsigned char)SHIFT_T_IND)
		xor_ind(arena, child, op_data);
	else
		xor_dir(child, op_data);
	return (0);
}

int			op_xor(t_operation *cmd_input, void *arena, uint8_t plid,
				t_process *child)
{
	int				i;
	unsigned char	byte;
	t_andop			op_data;

	bzero(&op_data, sizeof(t_andop));
	op_data.encbyte = cmd_input->encbyte;
	byte = cmd_input->encbyte;
	i = 0;
	while (byte)
	{
		op_data.arg = &((cmd_input->args)[i]);
		op_data.argi = i;
		if (!xor_decider(arena, child, &op_data))
			i++;
		byte = byte << 2;
	}
	*(op_data.dest) = op_data.val;
	if (!*(op_data.dest))
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
