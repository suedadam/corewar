/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_sti.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 18:07:00 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** (20370+130)%20480
** Read all the encoding for the right variables.
*/ 

// rev_write_memory(arena, child->pc, val, REG_SIZE);
int	op_sti(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	int				val;
	int				val2;
	int				j;
	unsigned char	byte;
	unsigned char	tmp;
	int				src;
	unsigned char	*dest;

	printf("{STI} %p\n", &src);
	j = 0;
	val = 0;
	byte = cmd_input->encbyte;
	while (byte)
	{
		tmp = byte & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_DIR)
		{
			if (op_tab[child->opcode - 1].encbool)
			{
				val += ((short)((cmd_input->args)[j]));
				printf("{TRUNC} IND = %d\n", (short)((cmd_input->args)[j]));
			}
			else
			{
				printf("DIR = %d\n", (cmd_input->args)[j]);
				val += (cmd_input->args)[j];				
			}

			//Insert solution here.
		}
		else if (tmp == (unsigned char)SHIFT_T_IND)
		{
			val2 = 0;
			copy_memory_fwd_off(&val2, arena, child, F_IND_SIZE, (cmd_input->args)[j]);
			val += (short)val2;
			printf("IND = %d\n", val2);
		}
		else
		{
			printf("REG\n");
			if (!j)
				src = (int)child->regs[(cmd_input->args)[j]];
			else
				val += child->regs[(cmd_input->args)[j]];
		}
		j++;
		byte = byte << 2;
	}
	printf("Src  - %d (%02x); offset = %d (%d)\n", src, src, val, child->pc + val % IDX_MOD);
	val2 = 0;
	if ((child->pc + val % IDX_MOD) < 0)
	{
		printf("so its not negative? %d %x %p\n", src, src, &src);
		src = ntohl(src);
		rev_write_memory(arena, (unsigned char *)&src, child->pc + val % IDX_MOD, REG_SIZE);
		// dest = ft_rev_mem_warp(arena, child->pc, val, REG_SIZE, &val2);
		// printf("%p (%p vs %p) (%d)\n", arena + child->pc, arena, dest, val2);
		// if (val2)
		// {
		// 	printf("cunt\n");
		// 	memcpy(dest, &src, val2);
		// 	dest = (arena + (MEM_SIZE - 1));
		// }
		// if (val == -186)
		// 	src = ~(src);
		// memcpy(dest, &src + val2, REG_SIZE - val2);
	}
	else
	{
		src = ntohl(src);
		write_memory(arena, (unsigned char *)&src, child->pc + val % IDX_MOD, REG_SIZE);
		// dest = ft_memory_warp(arena, child->pc, val, REG_SIZE, &val2);
		// if (val2)
		// {
		// 	memcpy(dest, &src, val2);
		// 	dest = arena;
		// 	// dest = arena + (MEM_SIZE - 1);
		// }
		// memcpy(dest, &src + val2, REG_SIZE - val2);
	}
	// dest = ft_memory_warp
	return (0);
}

// int	op_sti(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
// {
// 	ssize_t	seek;
// 	void	*wrap;
// 	int		bigE;
// 	int		frag;

// 	wrap = NULL;
// 	frag = 0;
// 	if (cmd_input->args[0] < 1 || cmd_input->args[0] > REG_NUMBER)
// 	{
// 		printf("Invalid register %d\n", cmd_input->args[0]);
// 		return (-1);
// 	}
// 	bigE = htonl(child->regs[cmd_input->args[0]]);
// 	seek = (short)cmd_input->args[1] + (short)cmd_input->args[2];
// 	// printf("seek mod = %d\n", seek % IDX_MOD);
// 	printf("Seek = %d total offset = %d\n", seek, child->pc + seek % IDX_MOD);
// 	wrap = ft_memory_warp(arena, child->pc, seek % IDX_MOD, REG_SIZE, &frag);
// 	if (frag)
// 	{
// 		memcpy(wrap, &bigE, frag);
// 		wrap = arena;
// 	}
// 	memcpy(wrap, &bigE + frag, REG_SIZE - frag);
// 	printf("{STI} = %d %x\n", ntohl(*(int *)wrap), *(int *)wrap);
// 	// exit(1);
// 	// int j;
// 	// unsigned char* byte_array = arena + child->pc;

// 	// j = 0;
// 	// while (j < 100)
// 	// {
// 	// 	printf("%02X ",(unsigned)byte_array[j]);
// 	// 	j++;
// 	// }
// 	// exit(1);
// 	return (0);
// }
