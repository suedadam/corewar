/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_and.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 11:28:48 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** circular memory :/
*/

int	op_and(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	unsigned char	tmp;
	unsigned char	byte;
	uint64_t		val1;
	int				*storage; //Max size :P 
	int				i;

	val1 = 0;
	byte = cmd_input->encbyte;
	i = 0;
	bzero(storage, REG_SIZE);
	while (byte)
	{
		tmp = byte;
		tmp = tmp & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_REG)
		{
			if (cmd_input->args[i] < 1 || cmd_input->args[i] > REG_NUMBER)
			{
				printf("Invalid register %d\n", cmd_input->args[i]);
				return (-1);
			}
			if (!i)
				val1 = (child->regs)[cmd_input->args[i]];
			else if (i == 1)
				val1 = val1 & (child->regs)[cmd_input->args[i]];
			else
			{
				printf("Found the storage register!\n");
				storage = &((child->regs)[cmd_input->args[i]]);
			}
		}
		else if (tmp == (unsigned char)SHIFT_T_IND)
		{
			if (!i)
				ft_memcpy(&val1, (arena + child->pc + (cmd_input->args)[i]), IND_SIZE);
			else if (i == 1)
			{
				storage = ft_memcpy(storage, (arena + child->pc + (cmd_input->args)[i]), IND_SIZE);
				val1 = val1 & *(uint16_t *)storage;
				bzero(storage, REG_SIZE);	
			}
			else
			{
				printf("Undefined?\n");
				exit(1);
			}
		}
		else if (tmp == (unsigned char)SHIFT_T_DIR)
		{
			if (!i)
				val1 = (cmd_input->args)[i];
			else if (i == 1)
				val1 = val1 & (cmd_input->args)[i];
			else
			{
				printf("undefined\n");
				exit(1);
			}
		}
		byte = byte << 2;
	}
	*storage = (int)val1;
	return (-1);
}