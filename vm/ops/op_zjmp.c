/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 15:12:25 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	int		frag;
	void	*new;

	frag = 0;
	if (child->carry == 1)
	{
		printf("carry present puta madre %d\n", (short)cmd_input->args[0]);
		printf("before = %p\n", arena + child->pc);
		if ((short)cmd_input->args[0] < 0)
		{
			//Negative.
			if ((short)cmd_input->args[0] & 0x8000 > child->pc)
			{
				child->pc = (MEM_SIZE - (child->pc + (short)cmd_input->args[0] & 0x8000));
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
		// new = ft_rev_mem_warp(arena, child->pc, (short)cmd_input->args[0])
		// if ((short)cmd_input->args[0] > child->pc)
		// {
		// 	printf("%d > %d\n", (short)cmd_input->args[0], child->pc);
		// 	//wrap downwards.
		// }
		// else
		// 	child->pc += ((short)cmd_input->args[0] % IDX_MOD);
		// printf("after = %p\n", arena + child->pc);
		// int j;
		// unsigned char* byte_array = arena + child->pc;
		// j = 0;
		// while (j < 100)
		// {
		// 	printf("%02X ",(unsigned)byte_array[j]);
		// 	j++;
		// }
		// exit(1);
	}
	return (0);
}