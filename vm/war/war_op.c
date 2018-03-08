/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:51:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/07 04:27:57 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	invalid_opcode(t_process *child)
{
	printf("(%d) {%d;%d} Invalid OPCode {%d}\n", taskmanager->currCycle, child->opcode, child->randID, child->opcode);
	if (child->pc == (MEM_SIZE - 1))
		child->pc = 0;
	else
		child->pc++;
	return (0);
}

static int	calc_enc_size(t_process *child, unsigned char byte)
{
	int	size;
	int argc;
	unsigned char tmp;

	argc = op_tab[child->opcode - 1].argc;
	size = 0;
	while (argc--)
	{
		tmp = byte & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_REG)
			size += F_REG_SIZE;
		else if (tmp == (unsigned char)SHIFT_T_DIR)
		{
			if (op_tab[child->opcode - 1].encbool)
				size += F_IND_SIZE;
			else
				size += F_DIR_SIZE;
		}
		else if (tmp == (unsigned char)SHIFT_T_IND)
			size += F_IND_SIZE;
		byte = byte << 2;
	}
	return (size);
}

static int	invalid_ACB(t_process *child, int size)
{
	printf("{%d} Invalid ACB\n", child->opcode);

	// 	negswap = ((short)cmd_input->args[0]);
	// if (negswap < 0)
	// 	child->pc = ((child->pc + negswap % IDX_MOD) + MEM_SIZE) % MEM_SIZE;
	// else
	// 	child->pc = (child->pc + negswap % IDX_MOD) % MEM_SIZE;
	if ((child->pc + size) < 0)
		child->pc = ((child->pc + size) + MEM_SIZE) % MEM_SIZE;
	else
		child->pc = (child->pc + size) % MEM_SIZE;
	child->opcode = 0;
	child->run_op = 0;
	// if (child->pc == (MEM_SIZE - 1))
	// 	child->pc = 0;
	// else
	// 	child->pc++;
	return (0);
}

static int	fetch_input(unsigned char *arena, t_operation *cmd_input, t_process *child)
{
	size_t			size;

	size = 1; //Skip the opcode.
	// printf("First byte = %02x\n", *(arena + size));
	if (op_tab[child->opcode - 1].encbyte[0] == T_DIR)
	{
		if (op_tab[child->opcode - 1].trunc)
		{
			(cmd_input->args)[0] = 0;
			copy_memory_fwd_off(&(cmd_input->args[0]), arena, child->pc + size, sizeof(short));
			// copy_memory_fwd_off(&((cmd_input->args)[0]), arena, child, sizeof(short), size);
			(cmd_input->args)[0] = ntohs((cmd_input->args)[0]);
			printf("Fetched indirect size = %d %x\n", (cmd_input->args)[0], (cmd_input->args)[0]);
			// exit(1);
			size += F_IND_SIZE;
		}
		else
		{
			(cmd_input->args)[0] = 0;
			copy_memory_fwd_off(&(cmd_input->args[0]), arena, child->pc + size, sizeof(int));
			// copy_memory_fwd_off(&((cmd_input->args)[0]), arena, child, sizeof(int), size);
			(cmd_input->args)[0] = ntohl((cmd_input->args)[0]);
			// (cmd_input->args)[0] = ntohl(*((int *)(p + size)));
			size += F_DIR_SIZE;
			printf("Fetched DIR %d %x\n", (cmd_input->args)[0], (cmd_input->args)[0]);
		}
	}
	else
	{
		// invalid_opcode(child);
		printf("Undefined? %s\n", op_tab[child->opcode - 1].op_name);
		exit(200);
		return (-1);
	}
	return (size);
}

/*
** If truncation then always fetch as T_IND
** Change this to be circular! <--- !!!
*/
//static int	copy_memory_fwd(void *dest, unsigned char *arena, t_process *child, size_t size);
// static int	copy_memory_fwd_off(void *dest, unsigned char *arena, t_process *child, size_t size, int offset)
static int	decode_ACB(unsigned char *arena, t_operation *cmd_input, t_process *child)
{
	int				j;
	size_t			size;
	unsigned char	byte;
	unsigned char	tmp;
	unsigned char	p[3];

	bzero(p, 3);
	copy_memory_fwd_off(p, arena, child->pc, 2);
	// printf("Hex of p inputs: %02x %d\n", p, (int)*p);
	cmd_input->encbyte = *(p + 1);
	byte = *(p + 1);
	j = 0;
	size = 2; //Skip the encoding byte.
	// printf("%02x vs %02x\n", byte, *(p + size));
	while (byte)
	{
		tmp = byte;
		tmp = byte & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_REG)
		{
			if ((op_tab[child->opcode - 1].encbyte[j] & T_REG) != T_REG)
			{
				// printf("Failed here \n");
				printf("(%d) Cunts UID: %d\n", taskmanager->currCycle, child->randID);
				invalid_opcode(child);
				return (-1);
			}
			(cmd_input->args)[j] = 0;
			copy_memory_fwd_off(&((cmd_input->args[j++])), arena, child->pc + size, sizeof(unsigned char));
			// copy_memory_fwd_off(&((cmd_input->args)[j++]), arena, child, 1, size);
			// (cmd_input->args)[j++] = *(unsigned char *)(p + size);
			size += F_REG_SIZE;
			printf("Fetched REG with val \"%d\" %x\n", (cmd_input->args)[j - 1], (cmd_input->args)[j - 1]);
			// exit(1);
		}
		else if (tmp == (unsigned char)SHIFT_T_DIR && !op_tab[child->opcode - 1].trunc)
		{
			if ((op_tab[child->opcode - 1].encbyte[j] & T_DIR) != T_DIR)
			{
				printf("(%d) O.o UID: %d\n", taskmanager->currCycle, child->randID);
				invalid_opcode(child);
				return (-1);
			}
			(cmd_input->args)[j] = 0;
			copy_memory_fwd_off(&((cmd_input->args)[j]), arena, child->pc + size, sizeof(int));
			// copy_memory_fwd_off(&((cmd_input->args)[j]), arena, child, sizeof(int), size);
			(cmd_input->args)[j] = ntohl((cmd_input->args)[j]);
			j++;
			// (cmd_input->args)[j++] = ntohl(*((int *)(p + size)));
			size += F_DIR_SIZE;
			printf("Fetched DIR withf val \"%d\" %x\n", (cmd_input->args)[j - 1], (cmd_input->args)[j - 1]);
			// exit(1);
		}
		else if (tmp == (unsigned char)F_IND_SIZE || (tmp == (unsigned char)SHIFT_T_DIR && op_tab[child->opcode - 1].trunc))
		{
			if (((op_tab[child->opcode - 1].encbyte[j] & T_IND) != T_IND) && !op_tab[child->opcode - 1].trunc)
			{
				printf("(%d) Penis UID: %d\n", taskmanager->currCycle, child->randID);
				invalid_opcode(child);
				return (-1);
			}
			(cmd_input->args)[j] = 0;
			copy_memory_fwd_off(&((cmd_input->args)[j]), arena, child->pc + size, sizeof(short));
			(cmd_input->args)[j] = ntohs((cmd_input->args)[j]);
			j++;
			// (cmd_input->args)[j++] = ntohs(*(short *)(p + size));
			size += F_IND_SIZE;
			// printf("Fetched IND with val \"%d\" %x\n", (short)(cmd_input->args)[j - 1], (cmd_input->args)[j - 1]);
		}
		byte = byte << 2;
	}
	if (cmd_input->encbyte << (2 * op_tab[child->opcode - 1].argc) == 00)
	{
		printf("(%d) {B} EMPTYFATLARD UID: %d (%02x) OPCode: (%02x vs) %02x %02x size = %d (%d)\n", taskmanager->currCycle, child->randID, cmd_input->encbyte, child->opcode, *(unsigned char *)(arena + child->pc), *(unsigned char *)(arena + child->pc + 1), size, child->pc);
		invalid_ACB(child, 2);
		printf("(%d) {A} EMPTYFATLARD UID: %d (%02x) size = %d (%d)\n", taskmanager->currCycle, child->randID, cmd_input->encbyte, size, child->pc);
		return (-1);
	}
	else if (j != op_tab[child->opcode - 1].argc)
	{
		// printf("w0w 3 many for me\n");
		printf("(%d) BITCHMOTHERFUCKA UID: %d (%02x) Pushing %d bytes!\n", taskmanager->currCycle, child->randID, cmd_input->encbyte, size + calc_enc_size(child, cmd_input->encbyte));
		printf("(%d) {B} {INV_ACB} %p : %d (%02x)\n", taskmanager->currCycle, arena + child->pc, child->pc, *(unsigned char *)(arena + child->pc));
		invalid_ACB(child, size + calc_enc_size(child, cmd_input->encbyte));
		printf("(%d) {A} {INV_ACB} %p : %d (%02x)\n", taskmanager->currCycle, arena + child->pc, child->pc, *(unsigned char *)(arena + child->pc));
		// invalid_opcode(child);
		return (-1);
	}
	return (size);
}

void	raincheck(void *arena, t_process *child, uint8_t penis)
{
	int	opcode;

	opcode = 0;
	copy_memory_fwd_off(&opcode, arena, child->pc, sizeof(unsigned char));
	// copy_memory_fwd(&opcode, (unsigned char *)arena, child, sizeof(unsigned char));
	if ((opcode > 15 || opcode <= 0))
	{
		printf("(%d) {RainCheck} UID: %d hmmm - raincheck? (OP: %d;%x) %d\n", taskmanager->currCycle, child->randID, opcode, opcode, child->pc);
		invalid_opcode(child);
		// invalid_opcode(child);
		// exit(300);
		// 		int j;
		// 		unsigned char* byte_array = arena + child->pc;

		// 		j = 0;
		// 		while (j < 1024)
		// 		// while (j < 1024)
		// 		{
		// 			printf("%02x ",(unsigned)byte_array[j]);
		// 			j++;
		// 		}
		// 		exit(1);
		// exit(1);
		return ;
		// return (-1);
	}
	else
	{
		printf("(%d) {RC} UID: %d Fetched OP: %d cycles = %d\n", taskmanager->currCycle, child->randID, opcode, op_tab[opcode - 1].waitcycles);
	}
	child->opcode = opcode;
	child->run_op = taskmanager->currCycle + op_tab[(opcode - 1)].waitcycles;
	// printf("CurrCycle: %d requested: %d Scheduled %d for %d\n", taskmanager->currCycle, op_tab[opcode - 1].waitcycles, opcode, child->run_op);
}

int	run_operation(int pID, void *arena, t_process *child)
{
	unsigned char	*ophex;
	int				opcode;
	t_operation		cmd_input;
	int				size;

	opcode = 0;
	if (!(child->run_op) || !(child->opcode))
		raincheck(arena, child, 1);
	// {
	// 	// printf("Copying memory from %d\n", child->pc);
	// 	copy_memory_fwd(&opcode, (unsigned char *)arena, child, sizeof(unsigned char));
	// 	// opcode = (int)(*(unsigned char *)(arena + child->pc));
	// 	if (opcode > 15 || opcode <= 0)
	// 	{
	// 		// printf("bitch? opcode = %d %x\n", (int)opcode, opcode);
	// 		invalid_opcode(child);
	// 		exit(1);
	// 		return (-1);
	// 	}
	// 	// printf("opcode = %d (%s)\n", opcode, op_tab[opcode - 1].op_name);
	// 	child->opcode = opcode;
	// 	child->run_op = taskmanager->currCycle + op_tab[opcode - 1].waitcycles; //Does this work?
	// 	// printf("will run at %d + %d = %d\n", taskmanager->currCycle, op_tab[opcode - 1].waitcycles, child->run_op);
	// }
	else if (child->run_op == taskmanager->currCycle)
	{
		copy_memory_fwd_off(&opcode, arena, child->pc, sizeof(char));
		// copy_memory_fwd(&opcode, (unsigned char *)arena, child, sizeof(char));
		// opcode = (int)(*(unsigned char *)(arena + child->pc));
		// printf("Runtime for %s\n", op_tab[opcode - 1].op_name);
		// printf("opcode %d == opcode = %d\n", child->opcode, opcode);
		if (op_tab[child->opcode - 1].encbool)
		{
			printf("AT LEAST I GOT HERE\n");
			if ((size = decode_ACB((unsigned char *)arena, &cmd_input, child)) == -1)
			{
				// invalid_ACB(child, 2);
				printf("PENIS MOTHER SUCKER\n");
				// exit(500);
				return (-1);
			}
		}
		else
			if ((size = fetch_input((unsigned char *)arena, &cmd_input, child)) == -1)
			{
				printf("Invalid fetched output\n");
				return (-1);
			}
		printf("{B} %p -- %d\n", arena + child->pc, child->pc);
		unsigned char* byte_array = (arena + child->pc);
		printf("Invalid opcode = %d\n", child->opcode);
		// printf("(%d) {B DISPATCH} %d\n", taskmanager->currCycle, child->pID);
		if (opdispatch[child->opcode - 1].func(&cmd_input, arena, pID, child) == -1)
		{
			printf("Invalid operation\n");
			exit(1);
			return (-1);
		}
		if (child->opcode != 9 || (child->opcode == 9 && !child->carry))
		{
			if ((child->pc + size) >= (MEM_SIZE - 1))
				child->pc = ((child->pc + size) % (MEM_SIZE - 1));
			else
				child->pc += size;			
		}
		// printf("(%d) {A DISPATCH} %d\n", taskmanager->currCycle, child->pID);
		printf("{A} %p -- %d\n", arena + child->pc, child->pc);
		child->run_op = 0;
		child->opcode = 0;
		// printf("==== ");
		// int j;

		// j = 0;
		// while (j < size)
		// {
		// 	printf("%02X ",(unsigned)byte_array[j]);
		// 	j++;
		// }
		// printf("=====\n");
		// printf("Next op = %x (%d)\n", *(unsigned char *)(arena + child->pc), *(unsigned char *)(arena + child->pc));
		printf("(%d) {WOW} wow penis\n", taskmanager->currCycle);
		raincheck(arena, child, 0); //Need a raincheck :) 
	}
	else
		printf("Scheduled for cycle %d (%d) (%d)\n", child->run_op, taskmanager->currCycle, child->pc);
	return (0);
}