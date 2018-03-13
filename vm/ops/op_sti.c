/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_sti.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 23:17:56 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	sti_reg(t_process *child, t_andop *op_data)
{
	if (!op_data->argi)
		op_data->dest = &(child->regs[*(op_data->arg)]);
	else if (op_data->argi == 1)
		op_data->val = child->regs[*(op_data->arg)];
	else
		op_data->val += child->regs[*(op_data->arg)];
	return (0);
}

static int	sti_ind(unsigned char *arena, t_process *child, t_andop *op_data)
{
	int	tmp;

	if (op_data->argi == 1)
	{
		tmp = 0;
		copy_memory_fwd_off(&tmp, arena,
				child->pc + *op_data->arg % IDX_MOD, sizeof(short));
		op_data->val = ft_shortswap(tmp);
	}
	else if (op_data->argi > 1)
	{
		tmp = 0;
		copy_memory_fwd_off(&tmp, arena,
				child->pc + *op_data->arg % IDX_MOD, sizeof(short));
		op_data->val += ft_shortswap(tmp);
	}
	return (0);
}

static int	sti_dir(t_andop *op_data)
{
	if (op_data->argi == 1)
		op_data->val = (short)*(op_data->arg);
	else if (op_data->argi > 1)
		op_data->val += (short)*(op_data->arg);
	return (0);
}

static int	sti_decider(void *arena, t_process *child, t_andop *op_data)
{
	unsigned char	byte;

	byte = (op_data->encbyte << (2 * op_data->argi) & 0xC0);
	if (byte == (unsigned char)SHIFT_T_REG)
		sti_reg(child, op_data);
	else if (byte == (unsigned char)SHIFT_T_IND)
		sti_ind(arena, child, op_data);
	else
		sti_dir(op_data);
	return (0);
}

int			op_sti(t_operation *cmd_input, void *arena,
				uint8_t plid, t_process *child)
{
	int				i;
	unsigned char	byte;
	int				byteswap;
	t_andop			op_data;

	UNUSED(plid);
	bzero(&op_data, sizeof(t_andop));
	op_data.encbyte = cmd_input->encbyte;
	byte = cmd_input->encbyte;
	i = 0;
	while (byte)
	{
		op_data.arg = &((cmd_input->args)[i]);
		op_data.argi = i;
		if (!sti_decider(arena, child, &op_data))
			i++;
		byte = byte << 2;
	}
	byteswap = ft_longswap(*(op_data.dest));
	write_memory(arena, (unsigned char *)&byteswap,
				MEM_WARP(child->pc + op_data.val % IDX_MOD), REG_SIZE);
	return (0);
}
