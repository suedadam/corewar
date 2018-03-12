/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_ldi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 13:00:45 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	ldi_reg(unsigned char *arena, t_process *child, t_andop *op_data)
{
	int	tmp;

	if (!op_data->argi)
		op_data->val = child->regs[*(op_data->arg)];
	else if (op_data->argi == 1)
		op_data->val += child->regs[*(op_data->arg)];
	else
		op_data->dest = &(child->regs[*(op_data->arg)]);
	return (0);
}

/*
** We're using them as Indexes so they have to be direct...not indirect
** even tho truncation :/
*/

static int	ldi_dir(unsigned char *arena, t_process *child, t_andop *op_data)
{
	int	tmp;

	if (!op_data->argi)
		op_data->val = (short)*(op_data->arg);
	else if (op_data->argi == 1)
		op_data->val += (short)*(op_data->arg);
	return (0);
}

static int	ldi_decoder(void *arena, t_process *child, t_andop *op_data)
{
	unsigned char	byte;

	byte = ((op_data->encbyte << (2 * op_data->argi)) & 0xC0);
	if (byte == (unsigned char)SHIFT_T_REG)
		ldi_reg(arena, child, op_data);
	else if (byte == (unsigned char)SHIFT_T_IND ||
			byte == (unsigned char)SHIFT_T_DIR)
		ldi_dir(arena, child, op_data);
	return (0);
}

int			op_ldi(t_operation *cmd_input, void *arena,
					uint8_t plid, t_process *child)
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
		if (!ldi_decoder(arena, child, &op_data))
			i++;
		byte = byte << 2;
	}
	*(op_data.dest) = 0;
	copy_memory_fwd_off(op_data.dest, arena,
		MEM_WARP(child->pc + op_data.val % IDX_MOD), sizeof(int));
	*(op_data.dest) = ft_longswap(*(op_data.dest));
	return (0);
}
