/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 19:02:54 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 15:06:27 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H
# include <errno.h>
# include "libft.h"
# include "coredef.h"
# include "core_structs.h"

# define FATAL_ERROR(msg)	fatal_error(msg)
# define DEFAULT_ERROR		FATAL_ERROR(strerror(errno))

/*
** asm_fsm.c
*/

void				fsm_run(t_parse *parse);
t_state				fsm_syntax_error(t_parse *parse);

/*
** asm_header.c
*/

t_state				fsm_build_header(t_parse *parse);

/*
**	asm_label.c
*/

t_state				fsm_build_label(t_parse *parse);
void				validate_label_chars(t_token *arg);

/*
** asm_load.c
*/

t_line				*load_file(char *file);
void				unload_lines(t_line *lines);

/*
** asm_op.c
*/

t_state				fsm_build_op(t_parse *parse);

/*
** asm_token.c
*/

void				destroy_tokens(t_token *tokens);
t_token				*tokenize(t_line *line, char *delim);

/*
** asm_util.c
*/

void				fatal_error(char *msg);
void				lexical_error(int line, int col);
int					reverse_bytes(int n, ssize_t size);
void				syntax_error(int line, int col, char *type, char *val);
int					trim(char *arg);

/*
** asm_validate_arg.c
*/

t_bool				read_reg(t_token *op, t_token *arg);
t_bool				read_direct(t_token *op, t_token *arg, t_bool truncate);
t_bool				read_indirect(t_token *op, t_token *arg);

/*
** asm_write.c
*/

void				write_file(t_parse *parse, char *file);

extern const t_op	g_ops[];
#endif
