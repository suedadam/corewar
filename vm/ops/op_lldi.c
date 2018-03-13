/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lldi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 23:13:10 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	lldi_reg(t_process *child, t_andop *op_data)
{
	if (!op_data->argi)
		op_data->val = child->regs[*(op_data->arg)];
	else if (op_data->argi == 1)
		op_data->val += child->regs[*(op_data->arg)];
	else
		op_data->dest = &(child->regs[*(op_data->arg)]);
	return (0);
}

static int	lldi_dir(t_andop *op_data)
{
	if (!op_data->argi)
		op_data->val = (short)*(op_data->arg);
	else if (op_data->argi == 1)
		op_data->val += (short)*(op_data->arg);
	return (0);
}

static int	lldi_decoder(t_process *child, t_andop *op_data)
{
	unsigned char	byte;

	byte = ((op_data->encbyte << (2 * op_data->argi)) & 0xC0);
	if (byte == (unsigned char)SHIFT_T_REG)
		lldi_reg(child, op_data);
	else if (byte == (unsigned char)SHIFT_T_IND ||
			byte == (unsigned char)SHIFT_T_DIR)
		lldi_dir(op_data);
	return (0);
}

int			op_lldi(t_operation *cmd_input, void *arena,
					uint8_t plid, t_process *child)
{
	int				i;
	unsigned char	byte;
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
		if (!lldi_decoder(child, &op_data))
			i++;
		byte = byte << 2;
	}
	*(op_data.dest) = 0;
	copy_memory_fwd_off(op_data.dest, arena,
		MEM_WARP(child->pc + op_data.val), sizeof(int));
	*(op_data.dest) = ft_longswap(*(op_data.dest));
	if (*(op_data.dest))
		child->carry = 1;
	else
		child->carry = 0;
	return (0);
}
