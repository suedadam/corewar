/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_fork.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 07:25:16 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

//Remove me
#include <time.h>
#include <stdlib.h>
//EO-Remove me.

int	op_fork(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	static int 	penis = 1;
	t_process	*new;
	short 		negswap;

	if (!(new = ft_memalloc(sizeof(t_process))))
	{
		printf("Memory issue ;c\n");
		return (-1);
	}
	printf("(%d) {FORK}\n", taskmanager->currCycle);
	memcpy(new, child, sizeof(t_process));
	if ((short)((cmd_input->args)[0]) < 0)
	{
		negswap = ~((short)cmd_input->args[0] + 1);
		if ((child->pc + (negswap % IDX_MOD)) < 0)
		{
			new->pc = (MEM_SIZE - 1) - ((negswap % IDX_MOD) - child->pc);
			// new->pc = (MEM_SIZE - (child->pc + (child->pc + (negswap % IDX_MOD)));
		}
		else
		{
			new->pc = child->pc + (short)((cmd_input->args)[0]) % IDX_MOD;
		}
	}
	else
	{
		if ((child->pc + (short)(cmd_input->args[0]) % IDX_MOD) >= (MEM_SIZE - 1))
			new->pc = ((child->pc + (short)(cmd_input->args[0]) % IDX_MOD) % (MEM_SIZE - 1));
		else
			new->pc = child->pc + (short)((cmd_input->args)[0]) % IDX_MOD;
	}
	new->randID = penis++;
	printf("(%d) Created UID: %d\n", taskmanager->currCycle, new->randID);
	new->pID = child->pID;
	new->next = taskmanager->processes;
	taskmanager->processes = new;
	t_process *childrenlol = taskmanager->processes;
	printf("\n======\n");
	while (childrenlol)
	{
		printf("ID: %d PC: %d\n", childrenlol->pID, childrenlol->pc);
		childrenlol = childrenlol->next;
	}
	printf("\n======\n");
	// exit(5);
	raincheck(arena, new, 1);
	return (0);
}
