/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 16:02:06 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	int		frag;
	void	*new;
	short	negswap;

	frag = 0;
	if (child->carry == 1)
	{
		printf("carry present puta madre %d\n", (short)cmd_input->args[0]);
		printf("before = %p\n", arena + child->pc);
		child->pc--;
		if ((short)cmd_input->args[0] < 0)
		{
			negswap = (short)cmd_input->args[0] & 0x8000;
			//Negative.
			if (negswap > child->pc)
			{
				child->pc = (MEM_SIZE - (child->pc + negswap));
			}
			else
			{
				child->pc += (short)cmd_input->args[0];
			}
		}
		else
		{
			if ((short)cmd_input->args[0] + child->pc >= MEM_SIZE)
				child->pc = (child->pc + (short)cmd_input->args[0]) % MEM_SIZE;
			else
				child->pc += (short)cmd_input->args[0];
			//Positive.
		}
	}
	return (0);
}