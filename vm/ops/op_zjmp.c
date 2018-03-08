/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/07 02:09:53 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_zjmp(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	int		frag;
	void	*new;
	short	negswap;

	frag = 0;
	printf("(%d) {B} {ZJMP} -> %d\n", taskmanager->currCycle, child->pc);
	if (child->carry == 1)
	{
		negswap = ((short)cmd_input->args[0]);
		if (negswap < 0)
			child->pc = ((child->pc + negswap % IDX_MOD) + MEM_SIZE) % MEM_SIZE;
		else
			child->pc = (child->pc + negswap % IDX_MOD) % MEM_SIZE;
		// printf("carry present puta madre %d\n", (short)cmd_input->args[0]);
		// printf("before = %p\n", arena + child->pc);
		// if ((short)cmd_input->args[0] < 0)
		// {
		// 	negswap = ~((short)cmd_input->args[0] + 1);
		// 	// negswap = ((short)cmd_input->args[0]) & 0x8000;
		// 	//Negative.
		// 	if (negswap > child->pc)
		// 	{
		// 		printf("gaetan\n");
		// 		child->pc = ((MEM_SIZE - 1) - (negswap - child->pc));
		// 	}
		// 	else
		// 	{
		// 		printf("penis %d > %d\n", (short)negswap, child->pc);
		// 		child->pc += (short)cmd_input->args[0];
		// 	}
		// }
		// else
		// {
		// 	if ((short)cmd_input->args[0] + child->pc >= (MEM_SIZE - 1))
		// 		child->pc = (child->pc + (short)cmd_input->args[0]) % (MEM_SIZE - 1);
		// 	else
		// 		child->pc += (short)cmd_input->args[0];
		// 	//Positive.
		// }
		// printf("After = %p\n", arena + child->pc);
	}
	printf("(%d) {A} {ZJMP} -> %d (%02x)\n", taskmanager->currCycle, child->pc, *((unsigned char *)(arena + child->pc)));
	return (0);
}