/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:51:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/07 02:11:40 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int	invalid_opcode(t_process *child)
{
	printf("{%d} Invalid OPCode\n", child->opcode);
	if (child->pc == (MEM_SIZE - 1))
		child->pc = 0;
	else
		child->pc++;
	return (0);
}

static int	invalid_ACB(t_process *child)
{
	printf("{%d} Invalid ACB\n", child->opcode);
	if (child->pc == (MEM_SIZE - 1))
		child->pc = 0;
	else
		child->pc++;
	return (0);
}

static int	fetch_input(unsigned char *p, t_operation *cmd_input, t_process *child)
{
	size_t	size;

	size = 1; //Skip the opcode.
	printf("First byte = %02x\n", *(p + size));
	if (op_tab[child->opcode - 1].encbyte[0] == T_DIR)
	{
		if (op_tab[child->opcode - 1].trunc)
		{
			(cmd_input->args)[0] = ntohs(*(short *)((unsigned char *)(p + size)));
			printf("Fetched IND %d\n", (short)(cmd_input->args)[0]);
			// printf("Fetched indirect size = %d\n", *(int *)(p + size));
			// exit(1);
			size += F_IND_SIZE;
		}
		else
		{
			(cmd_input->args)[0] = ntohl(*((int *)(p + size)));
			size += F_DIR_SIZE;
			printf("Fetched DIR %d\n", (cmd_input->args)[0]);
		}
	}
	else
	{
		// invalid_opcode(child);
		printf("Undefined? %s\n", op_tab[child->opcode - 1].op_name);
		return (-1);
	}
	return (size);
}

// /*
// ** We don't really need to handle anything other than T_DIR :/
// */

// static int fetch_input(unsigned char *enc, t_operation *cmd_input,
// 							uint8_t opcode, size_t mem_base)
// {
// 	size_t	size;

// 	size = 0;
// 	if (op_tab[opcode - 1].encbyte[0] == T_DIR)
// 	{
// 		if (op_tab[opcode - 1].trunc)
// 		{
// 			printf("fetched as IND %x\n", *(uint16_t *)enc);
// 			(cmd_input->args)[0] = ntohs(*(uint16_t *)enc);
// 			size = F_IND_SIZE;
// 		}
// 		else
// 		{
// 			printf("fetched as DIR %x\n", *(uint32_t *)enc);
// 			(cmd_input->args)[0] = ntohl(*(uint32_t *)enc);
// 			printf("playerID = %d\n", (cmd_input->args)[0]);
// 			size = F_DIR_SIZE;
// 		}
// 	}
// 	else if (op_tab[opcode - 1].encbyte[0] == T_IND)
// 	{
// 		(cmd_input->args)[0] = ntohs(*(uint16_t *)enc);
// 		size = F_IND_SIZE;
// 	}
// 	else if (op_tab[opcode - 1].encbyte[0] == T_REG)
// 	{
// 		(cmd_input->args)[0] = *(unsigned char *)enc;
// 		size = 1;
// 	}
// 	return (size);
// }

/*
** If truncation then always fetch as T_IND
** Change this to be circular! <--- !!!
*/

static int	decode_ACB(unsigned char *p, t_operation *cmd_input, t_process *child)
{
	int				j;
	size_t			size;
	unsigned char	byte;
	unsigned char	tmp;

	cmd_input->encbyte = *(p + 1);
	byte = *(p + 1);
	j = 0;
	size = 2; //Skip the encoding byte.
	printf("%02x vs %02x\n", byte, *(p + size));
	while (byte)
	{
		tmp = byte;
		tmp = byte & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_REG)
		{
			if ((op_tab[child->opcode - 1].encbyte[j] & T_REG) != T_REG)
			{
				printf("Failed here \n");
				invalid_opcode(child);
				return (-1);
			}
			(cmd_input->args)[j++] = *(unsigned char *)(p + size);
			size += F_REG_SIZE;
			printf("Fetched REG with val \"%d\"\n", (cmd_input->args)[j - 1]);
		}
		else if (tmp == (unsigned char)SHIFT_T_DIR && !op_tab[child->opcode - 1].trunc)
		{
			if ((op_tab[child->opcode - 1].encbyte[j] & T_DIR) != T_DIR)
			{
				invalid_opcode(child);
				return (-1);
			}
			(cmd_input->args)[j++] = ntohl(*((int *)(p + size)));
			size += F_DIR_SIZE;
			printf("Fetched DIR with val \"%d\" %x\n", (cmd_input->args)[j - 1], (cmd_input->args)[j - 1]);
		}
		else if (tmp == (unsigned char)F_IND_SIZE || (tmp == (unsigned char)SHIFT_T_DIR && op_tab[child->opcode - 1].trunc))
		{
			if (((op_tab[child->opcode - 1].encbyte[j] & T_IND) != T_IND) && !op_tab[child->opcode - 1].trunc)
			{
				invalid_opcode(child);
				return (-1);
			}
			(cmd_input->args)[j++] = ntohs(*(short *)(p + size));
			size += F_IND_SIZE;
			printf("Fetched IND with val \"%d\"\n", (cmd_input->args)[j - 1]);
		}
		byte = byte << 2;
	}
	if (j != op_tab[child->opcode - 1].argc)
	{
		invalid_opcode(child);
		return (-1);
	}
	return (size);
}

int	run_operation(int pID, void *arena, t_process *child)
{
	unsigned char	*ophex;
	int				opcode;
	t_operation		cmd_input;
	int				size;

	if (!(child->run_op))
	{
		opcode = (int)(*(unsigned char *)(arena + child->pc));
		if (opcode > 15 || opcode <= 0)
		{
			invalid_opcode(child);
			return (-1);
		}
		printf("opcode = %d (%s)\n", opcode, op_tab[opcode - 1].op_name);
		child->opcode = opcode;
		child->run_op = taskmanager->currCycle + op_tab[opcode - 1].waitcycles;
	}
	else if (child->run_op == taskmanager->currCycle)
	{
		opcode = (int)(*(unsigned char *)(arena + child->pc));
		printf("Runtime for %s\n", op_tab[opcode - 1].op_name);
		printf("opcode %d == opcode = %d\n", child->opcode, opcode);
		if (op_tab[child->opcode - 1].encbool)
		{
			if ((size = decode_ACB((unsigned char *)(arena + child->pc), &cmd_input, child)) == -1)
			{
				invalid_ACB(child);
				return (-1);
			}
		}
		else
			if ((size = fetch_input((unsigned char *)(arena + child->pc), &cmd_input, child)) == -1)
				return (-1);
		printf("{B} %p -- %d\n", arena + child->pc, child->pc);
		unsigned char* byte_array = (arena + child->pc);
		if (opdispatch[child->opcode - 1].func(&cmd_input, arena, pID, child) == -1)
		{
			printf("Invalid operation\n");
			return (-1);
		}
		if (child->opcode != 9)
		{
			if ((child->pc + size) >= (MEM_SIZE - 1))
				child->pc = ((child->pc + size) % (MEM_SIZE - 1));
			else
				child->pc += size;			
		}
		printf("{A} %p -- %d\n", arena + child->pc, child->pc);
		child->run_op = 0;
		child->opcode = 0;
		printf("====\n");
		int j;

		j = 0;
		while (j < size)
		{
			printf("%02X ",(unsigned)byte_array[j]);
			j++;
		}
		printf("=====\n");
	}
	return (0);
}

// int	run_operation(int pID, void *arena, t_process *child)
// {
// 	unsigned char	*ophex;
// 	unsigned char	*tmp;
// 	ssize_t			size;
// 	int				opcode;
// 	t_operation		cmd_input;

// 	// if (!((taskmanager->players)[pID]->processes->run_op))
// 	if (!(child->run_op))
// 	{
// 		ophex = (unsigned char *)(arena + child->pc);
// 		// ophex = (unsigned *)(arena + (taskmanager->players)[pID]->processes->pc);
// 		// printf("[ID: %d] Calling HextoDec(%02X)\n", pID, *ophex);
// 		opcode = (int)*ophex;
// 		if (opcode > 15 || opcode <= 0)
// 		{
// 			printf("Invalid opcode\n");
// 			if (child->pc == (MEM_SIZE - 1))
// 				child->pc = 0;
// 			else
// 				child->pc++;
// 			return (-1);
// 		}
// 		else
// 		{
// 			printf("Operation = %s code = %d\n", op_tab[opcode - 1].op_name, opcode);
// 			child->opcode = opcode;
// 			child->run_op = taskmanager->currCycle + op_tab[opcode - 1].waitcycles;
// 			if (child->pc == (MEM_SIZE - 1))
// 				child->pc = 0;
// 			else
// 				child->pc++;
// 		}
// 	}
// 	else if (child->run_op == taskmanager->currCycle)
// 	{
// 		printf("BAM MOTHER FUCKA\n");
// 		// ophex = (unsigned char *)(arena + (taskmanager->players)[pID]->processes->pc + 1); //Use this to skip validation of the opcode (not necessary).
// 		ophex = (unsigned char *)(arena + child->pc);
// 		opcode = child->opcode;
// 		printf("OPCODe = \"%s\"\n", op_tab[opcode - 1].op_name);
// 		if ((int)*ophex == opcode)
// 		{
// 			printf("Correct okay bitch\n");
// 			// ophex++;
// 			memory_move(child->pc, 1, &ophex, arena);
// 		}
// 		else
// 			printf("O_O :O;Still need to do thisssss\n");
// 		// child->pc++;
// 		if (op_tab[opcode - 1].encbool)
// 		{
// 			printf("Has an encoding byte!!!!\n");
// 			printf("{B} Enc = %p\n", ophex);
// 			if ((size = decode_encoding(ophex, &cmd_input, opcode, child->pc)) == -1)
// 			{
// 				printf("Bad encoding byte!\n");
// 				if (child->pc == (MEM_SIZE - 1))
// 					child->pc = 0;
// 				// else
// 				// 	child->pc++;
// 				// memory_move((taskmanager->players)[pID]->processes->pc, 1, ophex, arena);
// 				// (taskmanager->players)[pID]->processes->pc++;
// 			}
// 		}
// 		else
// 		{
// 			printf("No encoding byte :O\n");
// 			if ((size = fetch_input(ophex, &cmd_input, opcode, (taskmanager->players)[pID]->processes->pc)) == -1)
// 			{
// 				printf("Bad fetch_input\n");
// 				if (child->pc == (MEM_SIZE - 1))
// 					child->pc = 0;
// 				// else
// 				// 	child->pc++;
// 			}
// 		}
// 		if (opdispatch[opcode - 1].func(&cmd_input, arena, pID, child) == -1)
// 		{
// 			printf("Failed to execute the fckin command wtf ;_;\n");
// 			// child->pc++;
// 			size = -1;
// 		}
// 		else
// 		{
// 			printf("passed the command motherfuckaaaa %d\n", size);
// 		}
// 		// execute_command(&cmd_input, arena, pID, child);
// 		if (size >= 0)
// 		{
// 			printf("%llu + %zu > %d\n", child->pc, size, MEM_SIZE - 1);
// 			if ((child->pc + size) >= (MEM_SIZE - 1))
// 				child->pc = (size - (MEM_SIZE - child->pc));
// 			else
// 				child->pc += size;
// 		}
// 		ophex = (unsigned char *)(arena + child->pc);
// 		printf("next op = %d\n", (int)*ophex); 
// 		if (op_tab[opcode - 1].trunc)
// 		{
// 			printf("Fucking truncation\n");
// 		}
// 		child->run_op = 0;
// 		child->opcode = 0;
// 		printf("made it\n");
// 	printf("Printing arena: \n");
// 	int j;
// 	unsigned char* byte_array = arena + child->pc;

// 	j = 0;
// 	while (j < (MEM_SIZE - child->pc))
// 	{
// 		printf("%02X",(unsigned)byte_array[j]);
// 		j++;
// 	}
// 		// exit(1);
// 	}
// 	return (0);
// }
