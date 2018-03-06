/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 15:32:14 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 10:41:11 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VM_H
# define FT_VM_H
#include "libft/libft.h"
#include "op.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct		s_op
{
	char		*op_name;
	uint8_t		argc;
	uint8_t		encbyte[3];
	uint8_t		opcode;
	int			waitcycles;
	char		*desc;
	uint8_t 	encbool:1;
	uint8_t		trunc:1;
}					t_op;

/*
** OMG WHY DON'T WE HAVE GENERICS FUCKKKKKKKKKK D: ;_;
** FUcking hell just check each value to see if it is larger than blah size ;) 
** I mean that sucks but whatever ya know? We can haz a sanitization function boi 
*/

typedef struct	s_process
{
	int					regs[REG_NUMBER + 1]; // (I refuse to do reg - 1 each time).
	uint64_t			pc;
	uint8_t				carry; //What does this actually do?
	size_t				run_op;
	uint8_t				opcode;
	uint8_t				die_check;
	struct s_process	*next;
}				t_process;

typedef struct	s_player
{
	uint8_t		pID;
	t_process	*processes;
}				t_player;

typedef struct	s_taskmanager
{
	size_t		currCycle;
	size_t		lastnbrlive;
	uint32_t	totalPlayers;
	t_player	**players;
	int8_t		lastlive; //Default -1;
	size_t		c_to_die;
}				t_taskmanager;

typedef struct	s_operation
{
	unsigned char	encbyte;
	uint32_t		args[3];
}				t_operation;

typedef struct	s_opdispatch
{
	int			opcode;
	int 		(*func)(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
}				t_opdispatch;

/*
** main.c
*/

void			*init(int champc, char **champv);
void			*init_arena(void);
int				read_champion(char *filename, void *arena,
							int playerID);
int				init_scheduler(void);
int				add_scheduler(uint8_t opcode);

/*
** utils.c
*/

void			*ft_memory_warp(void *arena, uint64_t base, uint64_t seek, uint64_t size,
								int *frag);

/*
** war/
*/

int				init_war(void *arena);
int				run_operation(int pID, void *arena, t_process *child);
int 			HextoDec(unsigned char hex);

/*
** op/
*/

int				op_live(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_ld(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_st(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_add(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_sub(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_and(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_or(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_xor(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_zjmp(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_ldi(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_sti(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_fork(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_lld(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_lldi(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_lfork(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);
int				op_aff(t_operation *cmd_input, void *arena, uint8_t pID, t_process *child);


extern t_op				op_tab[17];
extern t_opdispatch		opdispatch[17];
extern t_taskmanager	*taskmanager;

#endif
