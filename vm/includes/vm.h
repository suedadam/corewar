/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 15:32:14 by asyed             #+#    #+#             */
/*   Updated: 2018/03/05 18:45:44 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VM_H
# define FT_VM_H
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct		s_op
{
	char		*op_name;
	uint8_t		argc;
	int			encbyte[3];
	uint8_t		opcode;
	int			waitcycles;
	char		*desc;
	uint8_t 	encbool:1;
	uint8_t		trunc:1;
}					t_op;

typedef struct	s_tasks
{
	size_t		run_op;
	uint8_t		opcode;
}				t_tasks;

typedef struct	s_scheduler
{
	t_tasks		*tasks;
}				t_scheduler;

typedef struct	s_process
{
	void		*regs;
	uint64_t	pc;
	uint8_t		carry; //What does this actually do? 
}				t_process;

typedef struct	s_taskmanager
{

}				t_taskmanager;

extern t_op			op_tab[17];
extern t_scheduler	scheduler;

#endif
