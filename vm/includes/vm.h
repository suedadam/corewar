/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 15:32:14 by asyed             #+#    #+#             */
/*   Updated: 2018/03/13 15:51:02 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VM_H
# define FT_VM_H
#include "libft.h"
#include "op.h"
#include "flags.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MEM_WARP(x) ((x + MEM_SIZE) % MEM_SIZE)

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

typedef struct	s_process
{
	uint8_t				plid;
	int					regs[REG_NUMBER + 1];
	int64_t				pc;
	uint8_t				carry;
	size_t				run_op;
	uint8_t				opcode;
	uint8_t				die_check;
	struct s_process	*next;
}				t_process;

typedef struct	s_andop
{
	int					*dest;
	int					val;
	int32_t				*arg;
	int					argi;
	unsigned char		encbyte;
}				t_andop;

typedef struct	s_taskmanager
{
	size_t		currCycle;
	void		*arena;
	size_t		lastnbrlive;
	uint32_t	totalPlayers;
	t_process	*processes;
	int8_t		lastlive;
	int			c_to_die;
	int			c_diecycles;
	int			c_checks;	
}				t_taskmanager;

typedef struct	s_operation
{
	unsigned char	encbyte;
	int32_t			args[3];
}				t_operation;

typedef struct	s_opdispatch
{
	int			opcode;
	int 		(*func)(t_operation *cmd_input, void *arena,
					uint8_t plid, t_process *child);
}				t_opdispatch;

/*
** fetcher/
*/

void			handle_reg(unsigned char *arena, t_process *child, 
							int32_t *storage, int *size);
void			handle_dir(unsigned char *arena, t_process *child, 
							int32_t *storage, int *size);
void			handle_ind(unsigned char *arena, t_process *child, 
							int32_t *storage, int *size);
int				fetch_decider(unsigned char *arena, t_operation *cmd_input,
						t_process *child, int j, int *size);

/*
** flags.c
*/

void			*fetch_result(char c, uint8_t convint);
int				flag_parse(int *argc, char ***argv);

/*
** invalid.c
*/
int				invalid_acb(unsigned char *arena, t_process *child, int size);
int				invalid_opcode(t_process *child);

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
** op/
*/

int				op_live(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_ld(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_st(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_add(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_sub(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_and(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_or(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_xor(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_zjmp(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_ldi(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_sti(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_fork(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_lld(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_lldi(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_lfork(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);
int				op_aff(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child);

/*
** utils.c
*/

void			copy_memory_fwd_off(void *dst, unsigned char *src, int offset, int size);
void			dump_memory(unsigned char *arena);
uint32_t		ft_longswap(uint32_t const byte);
uint16_t		ft_shortswap(uint16_t const byte);



/*
** war/
*/

int				init_war(void *arena);
int				run_operation(int plid, unsigned char *arena, t_process *child);
int 			HextoDec(unsigned char hex);
void			raincheck(void *arena, t_process *child);

void 			rev_write_memory(void *arena, unsigned char *src, int offset, int size);
void			write_memory(void *arena, unsigned char *src, int offset, int size);


extern t_op				g_op_tab[17];
extern t_opdispatch		g_opdispatch[17];
extern t_taskmanager	*g_taskmanager;
extern unsigned char	*g_arena;

#endif
