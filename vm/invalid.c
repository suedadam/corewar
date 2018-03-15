/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 16:40:21 by asyed             #+#    #+#             */
/*   Updated: 2018/03/15 00:46:24 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	invalid_opcode(t_process *child)
{
	if (child->pc == (MEM_SIZE - 1))
		child->pc = 0;
	else
		child->pc++;
	child->opcode = 0;
	child->run_op = 0;
	return (0);
}

int	invalid_acb(t_byte *arena, t_process *child, int size)
{
	printf("ADV %d (0x%04llx -> 0x%04llx) ", size, child->pc, MEM_WARP(child->pc + size));
 	int start = child->pc;
	int end = size;
	while (end--)
 	{
 		printf("%02x ", *(unsigned char *)(arena + MEM_WARP(start)));
 		start++;
 	}
 	printf("\n");
	child->pc = MEM_WARP(child->pc + size);
	child->opcode = 0;
	child->run_op = 0;
	raincheck(arena, child);
	return (0);
}
