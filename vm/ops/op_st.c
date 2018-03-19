/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_st.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 18:26:09 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	st_reg(t_process *child, t_andop *op_data)
{
	if (!op_data->argi)
		op_data->val = child->regs[*(op_data->arg)];
	else if (op_data->argi == 1)
	{
		child->regs[*(op_data->arg)] = op_data->val;
		op_data->dest = NULL;
	}
	return (0);
}

static int	st_decider(t_process *child, t_andop *op_data)
{
	t_byte	byte;

	byte = (op_data->encbyte << (2 * op_data->argi) & 0xC0);
	if (byte == (t_byte)SHIFT_T_REG)
		st_reg(child, op_data);
	else
		op_data->dest = op_data->arg;
	return (0);
}

int			op_st(t_operation *cmd_input, void *arena, t_process *child)
{
	int		i;
	int		byteswap;
	t_byte	byte;
	t_andop	op_data;

	bzero(&op_data, sizeof(t_andop));
	op_data.encbyte = cmd_input->encbyte;
	byte = cmd_input->encbyte;
	i = 0;
	while (byte)
	{
		op_data.arg = &((cmd_input->args)[i]);
		op_data.argi = i;
		if (!st_decider(child, &op_data))
			i++;
		byte = byte << 2;
	}
	if (op_data.dest)
	{
		byteswap = ft_longswap(op_data.val);
		write_memory(arena, (t_byte *)&byteswap,
					MEM_WARP(child->pc + (short)*op_data.dest % IDX_MOD), REG_SIZE);
	}
	return (0);
}
