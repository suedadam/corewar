/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 13:59:15 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	if (child->carry == 1)
	{
		printf("carry present puta madre %d\n", cmd_input->args[0]);
		printf("before = %p\n", arena + child->pc);
		child->pc += (cmd_input->args[0] % IDX_MOD);
		printf("after = %p\n", arena + child->pc);
		int j;
		unsigned char* byte_array = arena + child->pc;

		j = 0;
		while (j < 100)
		{
			printf("%02X ",(unsigned)byte_array[j]);
			j++;
		}
		exit(1);
	}
	return (0);
}