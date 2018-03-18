/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 16:12:03 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 16:13:02 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VM_H
# define FT_VM_H
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft.h"
# include "flags.h"
# include "op.h"

# define MEM_WARP(x) ((x + MEM_SIZE) % MEM_SIZE)

typedef struct	s_op
{
	char				*op_name;
	uint8_t				argc;
	uint8_t				encbyte[3];
	uint8_t				opcode;
	int					waitcycles;
	char				*desc;
	uint8_t				encbool:1;
	uint8_t				trunc:1;
}				t_op;

typedef struct	s_process
{
	uint8_t				plid;
	int32_t				regs[REG_NUMBER + 1];
	int64_t				pc;
	uint8_t				carry;
	size_t				run_op;
	uint8_t				opcode;
	uint8_t				die_check;
	struct s_process	*next;
}				t_process;

typedef struct	s_andop
{
	int32_t				*dest;
	int					val;
	int32_t				*arg;
	int					argi;
	t_byte				encbyte;
}				t_andop;

typedef struct	s_taskmanager
{
	size_t				curr_cycle;
	void				*arena;
	size_t				lastnbrlive;
	uint32_t			total_players;
	t_process			*processes;
	int8_t				lastlive;
	int					c_to_die;
	int					c_diecycles;
	int					c_checks;
}				t_taskmanager;

typedef struct	s_operation
{
	t_byte				encbyte;
	int32_t				args[3];
	int					size;
	int					ac;
}				t_operation;

typedef struct	s_opdispatch
{
	int					opcode;
	int					(*func)(t_operation *cmd_input, void *arena,
							t_process *child);
}				t_opdispatch;

/*
** fetcher/
*/

int				handle_reg(t_byte *arena, t_process *child, int32_t *storage,
					int *size);
void			handle_dir(t_byte *arena, t_process *child, int32_t *storage,
					int *size);
void			handle_ind(t_byte *arena, t_process *child, int32_t *storage,
					int *size);
int				fetch_decider(t_byte *arena, t_operation *cmd,
					t_process *child);

/*
** flags.c
*/

void			*fetch_result(char c, uint8_t convint);
int				flag_parse(int *argc, char ***argv);

/*
** invalid.c
*/
int				invalid_acb(t_byte *arena, t_process *child, int size);
int				invalid_opcode(t_process *child);

/*
** main.c
*/

void			*init(int champc, char **champv);
void			*init_arena(void);
int				read_champion(char *filename, void *arena, int plid);
int				init_scheduler(void);
int				add_scheduler(uint8_t opcode);

/*
** op/
*/

int				op_live(t_operation *cmd_input, void *arena, t_process *child);
int				op_ld(t_operation *cmd_input, void *arena, t_process *child);
int				op_st(t_operation *cmd_input, void *arena, t_process *child);
int				op_add(t_operation *cmd_input, void *arena, t_process *child);
int				op_sub(t_operation *cmd_input, void *arena, t_process *child);
int				op_and(t_operation *cmd_input, void *arena, t_process *child);
int				op_or(t_operation *cmd_input, void *arena, t_process *child);
int				op_xor(t_operation *cmd_input, void *arena, t_process *child);
int				op_zjmp(t_operation *cmd_input, void *arena, t_process *child);
int				op_ldi(t_operation *cmd_input, void *arena, t_process *child);
int				op_sti(t_operation *cmd_input, void *arena, t_process *child);
int				op_fork(t_operation *cmd_input, void *arena, t_process *child);
int				op_lld(t_operation *cmd_input, void *arena, t_process *child);
int				op_lldi(t_operation *cmd_input, void *arena, t_process *child);
int				op_lfork(t_operation *cmd_input, void *arena, t_process *child);
int				op_aff(t_operation *cmd_input, void *arena, t_process *child);

/*
** utils.c
*/

void			copy_memory_fwd_off(void *dst, t_byte *src, int off, int size);
void			dump_memory(t_byte *arena);
uint32_t		ft_longswap(uint32_t const byte);
uint16_t		ft_shortswap(uint16_t const byte);

/*
** war/
*/

void			init_war(void *arena);
int				run_operation(t_byte *arena, t_process *child);
int				htod(t_byte hex);
void			raincheck(void *arena, t_process *child);
void			rev_write_memory(void *arena, t_byte *src, int off, int size);
void			write_memory(void *arena, t_byte *src, int offset, int size);

extern t_op				g_op_tab[17];
extern t_opdispatch		g_opdispatch[17];
extern t_taskmanager	*g_taskmanager;
extern t_byte			*g_arena;
#endif
