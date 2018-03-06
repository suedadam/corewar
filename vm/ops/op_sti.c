/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_sti.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 13:43:49 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
/*
** (20370+130)%20480
** Read all the encoding for the right variables.
*/ 

int	op_sti(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	ssize_t	seek;
	void	*wrap;
	int		frag;

	wrap = NULL;
	frag = 0;
	if (cmd_input->args[0] < 1 || cmd_input->args[0] > REG_NUMBER)
	{
		printf("Invalid register %d\n", cmd_input->args[0]);
		return (-1);
	}
	seek = cmd_input->args[1] + cmd_input->args[2];
	// printf("seek mod = %d\n", seek % IDX_MOD);
	wrap = ft_memory_warp(arena, child->pc, seek % IDX_MOD, REG_SIZE, &frag);
	if (frag)
	{
		// printf("peni\n");
		memcpy(wrap, &(child->regs[cmd_input->args[0]]), frag);
		wrap = arena;
	}
	memcpy(wrap, &(child->regs[cmd_input->args[0]]) + frag, REG_SIZE - frag);
	// int j;
	// unsigned char* byte_array = arena + child->pc;

	// j = 0;
	// while (j < 100)
	// {
	// 	printf("%02X ",(unsigned)byte_array[j]);
	// 	j++;
	// }
	// exit(1);
	return (0);
}
