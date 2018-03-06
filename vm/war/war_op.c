/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:51:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 13:42:26 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** We don't really need to handle anything other than T_DIR :/
*/

static int fetch_input(unsigned char *enc, t_operation *cmd_input,
							uint8_t opcode, size_t mem_base)
{
	size_t	size;

	size = 0;
	if (op_tab[opcode - 1].encbyte[0] == T_DIR)
	{
		if (op_tab[opcode - 1].trunc)
		{
			printf("fetched as IND %x\n", *(uint16_t *)enc);
			(cmd_input->args)[0] = ntohs(*(uint16_t *)enc);
			size = F_IND_SIZE;
		}
		else
		{
			printf("fetched as DIR %x\n", *(uint32_t *)enc);
			(cmd_input->args)[0] = ntohl(*(uint32_t *)enc);
			printf("playerID = %d\n", (cmd_input->args)[0]);
			size = F_DIR_SIZE;
		}
	}
	else if (op_tab[opcode - 1].encbyte[0] == T_IND)
	{
		(cmd_input->args)[0] = ntohs(*(uint16_t *)enc);
		size = F_IND_SIZE;
	}
	else if (op_tab[opcode - 1].encbyte[0] == T_REG)
	{
		(cmd_input->args)[0] = *(unsigned char *)enc;
		size = 1;
	}
	return (size);
}

/*
** If truncation then always fetch as T_IND
** Change this to be circular! <--- !!!
*/

static int decode_encoding(unsigned char *enc, t_operation *cmd_input,
							uint8_t opcode, size_t mem_base)
{
	int				i;
	int				j;
	size_t			size;
	unsigned char	tmp;
	unsigned char	byte;

	i = 0;
	j = 0;
	cmd_input->encbyte = *enc;
	byte = *enc;
	printf("%x\n", byte);
	enc++;
	size = 1;
	while (byte)
	{
		tmp = byte;
		tmp = tmp & 0xC0;
		if (tmp == (unsigned char)SHIFT_T_REG)
		{
			(cmd_input->args)[j++] = *(unsigned char *)enc++;
			size++;
			printf("REG\n");
		}
		//Combine these two for checking truncation flag
		else if (tmp == (unsigned char)SHIFT_T_DIR)
		{
			if (op_tab[opcode - 1].trunc)
			{
				// bzero(tmpbuf, 1024);
				// ft_memcpy(tmpbuf, enc, F_IND_SIZE);
				// tmpbuf[F_IND_SIZE] = '\0';
				// printf("Fetched as IND {TRUNC}: %02x vs %x\n", (unsigned)*tmpbuf, (*(uint16_t *)tmpbuf));
				(cmd_input->args)[j++] = ntohs(*(uint16_t *)enc);
				printf("%x vs %llu\n", (cmd_input->args)[j - 1], (cmd_input->args)[j - 1]);
				enc += F_IND_SIZE;
				size += F_IND_SIZE;
				printf("IND {TRUNC}\n");
			}
			else
			{
				printf("Fetched as DIR: %x\n", (uint32_t)enc);
				(cmd_input->args)[j++] = ntohl(*(uint32_t *)enc);
				enc += F_DIR_SIZE;
				size += F_DIR_SIZE;
				printf("DIR\n");				
			}
		}
		else if (tmp == (unsigned char)SHIFT_T_IND)
		{
			printf("Fetched as IND: %x\n", (uint16_t)enc);
			(cmd_input->args)[j++] = ntohs(*(uint16_t *)enc);
			enc += F_IND_SIZE;
			size += F_IND_SIZE;
			printf("IND\n");
		}
		byte = byte << 2;
		i++;
	}
	if (i != op_tab[opcode - 1].argc) //Skip one byte.
	{
		printf("%d is a FAILUREEEEE\n", i);
		return (-1);
	}
	printf("i = %d vs %d\n", i, op_tab[opcode - 1].argc);
	printf("Enc = %p\n", enc);
	return (size);
}

static int	memory_move(uint64_t base, size_t j_size, unsigned char **move_pointer, void *arena)
{
	if (base + j_size >= MEM_SIZE)
	{
		(*move_pointer) = (arena + (j_size - ((uint64_t)MEM_SIZE - base)));
		return (1);
	}
	else
	{
		(*move_pointer) += j_size;
		return (0);
	}
}

int	run_operation(int pID, void *arena, t_process *child)
{
	unsigned char	*ophex;
	unsigned char	*tmp;
	ssize_t			size;
	int				opcode;
	t_operation		cmd_input;

	// if (!((taskmanager->players)[pID]->processes->run_op))
	if (!(child->run_op))
	{
		ophex = (unsigned char *)(arena + child->pc);
		// ophex = (unsigned *)(arena + (taskmanager->players)[pID]->processes->pc);
		// printf("[ID: %d] Calling HextoDec(%02X)\n", pID, *ophex);
		opcode = (int)*ophex;
		if (opcode > 15 || opcode <= 0)
		{
			printf("Invalid opcode\n");
			if (child->pc == (MEM_SIZE - 1))
				child->pc = 0;
			else
				child->pc++;
			return (-1);
		}
		else
		{
			printf("Operation = %s code = %d\n", op_tab[opcode - 1].op_name, opcode);
			child->opcode = opcode;
			child->run_op = taskmanager->currCycle + op_tab[opcode - 1].waitcycles;
			if (child->pc == (MEM_SIZE - 1))
				child->pc = 0;
			else
				child->pc++;
		}
	}
	else if (child->run_op == taskmanager->currCycle)
	{
		printf("BAM MOTHER FUCKA\n");
		// ophex = (unsigned char *)(arena + (taskmanager->players)[pID]->processes->pc + 1); //Use this to skip validation of the opcode (not necessary).
		ophex = (unsigned char *)(arena + child->pc);
		opcode = child->opcode;
		printf("OPCODe = \"%s\"\n", op_tab[opcode - 1].op_name);
		if ((int)*ophex == opcode)
		{
			printf("Correct okay bitch\n");
			// ophex++;
			memory_move(child->pc, 1, &ophex, arena);
		}
		else
			printf("O_O :O;Still need to do thisssss\n");
		// child->pc++;
		if (op_tab[opcode - 1].encbool)
		{
			printf("Has an encoding byte!!!!\n");
			printf("{B} Enc = %p\n", ophex);
			if ((size = decode_encoding(ophex, &cmd_input, opcode, child->pc)) == -1)
			{
				printf("Bad encoding byte!\n");
				if (child->pc == (MEM_SIZE - 1))
					child->pc = 0;
				// else
				// 	child->pc++;
				// memory_move((taskmanager->players)[pID]->processes->pc, 1, ophex, arena);
				// (taskmanager->players)[pID]->processes->pc++;
			}
		}
		else
		{
			printf("No encoding byte :O\n");
			if ((size = fetch_input(ophex, &cmd_input, opcode, (taskmanager->players)[pID]->processes->pc)) == -1)
			{
				printf("Bad fetch_input\n");
				if (child->pc == (MEM_SIZE - 1))
					child->pc = 0;
				// else
				// 	child->pc++;
			}
		}
		if (opdispatch[opcode - 1].func(&cmd_input, arena, pID, child) == -1)
		{
			printf("Failed to execute the fckin command wtf ;_;\n");
			// child->pc++;
			size = -1;
		}
		else
		{
			printf("passed the command motherfuckaaaa %d\n", size);
		}
		// execute_command(&cmd_input, arena, pID, child);
		if (size >= 0)
		{
			printf("%llu + %zu > %d\n", child->pc, size, MEM_SIZE - 1);
			if ((child->pc + size) >= (MEM_SIZE - 1))
				child->pc = (size - (MEM_SIZE - child->pc));
			else
				child->pc += size;
		}
		ophex = (unsigned char *)(arena + child->pc);
		printf("next op = %d\n", (int)*ophex); 
		if (op_tab[opcode - 1].trunc)
		{
			printf("Fucking truncation\n");
		}
		child->run_op = 0;
		child->opcode = 0;
		printf("made it\n");
	printf("Printing arena: \n");
	int j;
	unsigned char* byte_array = arena + child->pc;

	j = 0;
	while (j < (MEM_SIZE - child->pc))
	{
		printf("%02X",(unsigned)byte_array[j]);
		j++;
	}
		// exit(1);
	}
	return (0);
}
