/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 19:02:54 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 03:02:04 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H
# include "libft.h"
# include <errno.h>
# include <stdio.h> // REMOVE BEFORE SUBMISSION

# define FATAL_ERROR(msg)	fatal_error(msg)
# define DEFAULT_ERROR		FATAL_ERROR(strerror(errno))

# define PROG_NAME_LENGTH	128
# define COMMENT_LENGTH		2048
# define COREWAR_EXEC_MAGIC	0x00EA83F3
# define COMMENT_CHAR		'#'
# define LABEL_CHAR			':'
# define DIRECT_CHAR		'%'
# define SEPARATOR_CHAR		','
# define LABEL_CHARS		"abcdefghijklmnopqrstuvwxyz_0123456789"
# define NAME_CMD_STRING	".name"
# define COMMENT_CMD_STRING	".comment"

typedef enum		e_event
{
	EV_LABEL,
	EV_OP,
	EV_PERIOD,
	NEVENTS
}					t_event;

typedef enum		e_symbol
{
	SYM_COMMAND_NAME,
	SYM_COMMAND_COMMENT,
	SYM_DIRECT,
	SYM_DLABEL,
	SYM_IND,
	SYM_INDLABEL,
	SYM_LABEL,
	SYM_OP,
	SYM_REGISTER,
	UNDEFINED,
	NSYMBOLS
}					t_symbol;

typedef enum		e_state
{
	BUILD_HEADER,
	BUILD_LABEL,
	BUILD_OP,
	QUIT,
	START,
	NSTATES
}					t_state;

typedef struct		s_header
{
	unsigned int	magic;
	char			name[PROG_NAME_LENGTH + 1];
	unsigned short	size;
	char			desc[COMMENT_LENGTH + 1];
}					t_header;

typedef struct		s_line
{
	char			*data;
	int				col_num;
	int				line_num;
	struct s_line	*next;
}					t_line;

typedef struct		s_token
{
	char			*data;
	t_symbol		type;
	int				len;
	int				col_num;
	int				line_num;
	struct s_token	*next;
}					t_token;

typedef struct		s_parse
{
	t_header		header;
	t_line			*lines;
	t_token			*tokens;
	t_token			*curr;
	t_state			state;
	t_event			event;
}					t_parse;

/*
** fsm.c
*/

void				fsm_run(t_parse *parse);
t_state				fsm_syntax_error(t_parse *parse);

/*
** fsm_header.c
*/

t_state				fsm_build_header(t_parse *parse);

/*
**	fsm_label.c
*/

t_state				fsm_build_label(t_parse *parse);

/*
** fsm_op.c
*/

t_state				fsm_build_op(t_parse *parse);

/*
** load.c
*/

t_line				*load_file(char *file);
void				unload_lines(t_line *lines);

/*
** token.c
*/

void				destroy_tokens(t_token *tokens);
t_token				*tokenize(t_line *line, char *delim);

/*
** util.c
*/

void				fatal_error(char *msg);
void				lexical_error(int line, int col);
unsigned int		reverse_bytes(unsigned int n, ssize_t size);
void				syntax_error(int line, int col, char *type, char *val);
int					trim(char *arg);
#endif
