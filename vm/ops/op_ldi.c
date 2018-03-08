/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_ldi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 07:25:20 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_ldi(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child)
{
	unsigned char	tmp;
	unsigned char	byte;
	int				val;
	int				val2;
	int				j;

	// val = 0;
	// if ((child->pc + cmd_input->args[0] + cmd_input->args[1] % IDX_MOD) >= 0)
	// {
	// 	printf("yassss %d\n", child->pc + cmd_input->args[0] + cmd_input->args[1] % IDX_MOD);
	// 	copy_memory_fwd_off(&val, arena, child, IND_SIZE, child->pc + cmd_input->args[0] + cmd_input->args[1] % IDX_MOD);
	// }
	// else
	// 	copy_memory_fwd_off(&val, arena, child, IND_SIZE, MEM_SIZE - (child->pc + cmd_input->args[0] + cmd_input->args[1] % IDX_MOD));
	// // val = cmd_input->args[0] + cmd_input->args[1];
	// val2 = 0;
	// if ((child->pc + val % IDX_MOD) >= 0)
	// {
	// 	printf("OFRWARFJAOIF %d\n", child->pc + val % IDX_MOD);
	// 	copy_memory_fwd_off(&val2, arena, child, REG_SIZE, child->pc + val % IDX_MOD);
	// }
	// else
	// 	copy_memory_fwd_off(&val2, arena, child, REG_SIZE, MEM_SIZE - (child->pc + val % IDX_MOD));
	// child->regs[cmd_input->args[2]] = val2;
	byte = cmd_input->encbyte;
	val = 0;
	j = 0;
	while (byte)
	{
		tmp = byte & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_DIR || tmp == (unsigned char)SHIFT_T_IND)
		{
			if (!j)
			{
				val = cmd_input->args[0];
				// copy_memory_fwd_off(&val, arena, child, IND_SIZE, child->pc + (cmd_input->args)[0] % IDX_MOD);
				// val = (cmd_input->args)[0];
				printf("First = %d\n", val);
				// exit(12);
			}
			else
			{
				printf("2nd = %d %x\n", (short)(cmd_input->args)[1], (cmd_input->args)[1]);
				val += (short)((cmd_input->args)[1]);
			}
		}
		else if (tmp == (unsigned char)SHIFT_T_REG)
		{
			if (!j)
				val = child->regs[(cmd_input->args)[0]];
			else if (((byte << 2) & 0xC0) | 0)
			{
				printf("2nd op? %x %d\n", tmp, (cmd_input->args)[1]);
				val += child->regs[(cmd_input->args)[1]];
			}
			else
			{
				// printf("time to write over it.\n");
				printf("3rd is a register\n");
				val2 = 0;
				printf("Copying mod offset: %d\n", child->pc + val % IDX_MOD);
				printf("Offset on board = %02x %02x\n", *(unsigned char *)(arena + (child->pc + val % IDX_MOD)), *(unsigned char *)(1 + arena + (child->pc + val % IDX_MOD)));
				// void	test_copy_memory_fwd_off(void *dst, void *arena, unsigned char *arena, int offset, int size)
				test_copy_memory_fwd_off(&val2, arena, child->pc + val % IDX_MOD, REG_SIZE);
				// copy_memory_fwd_off(&val2, arena, child, REG_SIZE, child->pc + val % IDX_MOD);
				child->regs[(cmd_input->args)[2]] = htonl(val2);
				// val2 = 0;
				// copy_memory_fwd_off(&val2, arena, child, REG_SIZE, val % IDX_MOD);
			}
		}
		printf("another juan %x\n", tmp);
		j++;
		byte = byte << 2;
	}
	printf("Jump to me = %d\n", child->pc + val % IDX_MOD);
	printf("val I'm copying: %02x %02x %d %x vs %02x vs %x\n", (unsigned char)val2, (unsigned char)(val2 + 1), val2, val2, child->regs[(cmd_input->args)[2]], ntohl(child->regs[(cmd_input->args)[2]]));
	// child->regs[(cmd_input->args)[2]] = val2;
	// child->carry = 1;
	// printf("First: %d 2nd one: %d\n", );
	// printf("Register = %x\n", child->regs[(cmd_input->args)[2]]);
	// exit(1);
	// printf("%p %p %d\n", arena, &(child->regs[(cmd_input->args)[2]]), val % IDX_MOD);

	// if (val % IDX_MOD < 0)
	// 	rev_write_memory(arena, &(child->regs[(cmd_input->args)[2]]), val % IDX_MOD, REG_SIZE);
	// else	
	// 	write_memory(arena, &(child->regs[(cmd_input->args)[2]]), val % IDX_MOD, REG_SIZE);
	// printf("Reg val = %d %02x\n", child->regs[(cmd_input->args)[2]], val2);
	// exit(1);
	return (0);
}