/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 19:02:54 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/07 12:10:34 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H
# include "libft.h"
# include <errno.h>
# include <stdio.h> // REMOVE BEFORE SUBMISSION

# define FATAL_ERROR(msg)	fatal_error(msg)
# define DEFAULT_ERROR		FATAL_ERROR(strerror(errno))

# define COMMENT_CHAR		'#'

typedef enum	e_event
{
	EV_COLON,
	EV_COMMENT,
	EV_DIRECT,
	EV_ALPHA,
	EV_NEWLINE,
	EV_NULL_TERM,
	EV_NUMBER,
	EV_PERIOD,
	EV_REGISTER,
	EV_WHITESP,
	NEVENTS
}				t_event;

typedef enum	e_symbol
{
	SYM_DIRECT,
	SYM_DLABEL,
	SYM_INDIRECT,
	SYM_LABEL,
	SYM_OP,
	SYM_REGISTER,
	UNDEFINED,
	NSYMBOLS
}				t_symbol;

typedef enum	e_state
{
	BUILD_DLABEL,
	BUILD_SYM,
	CAT_LABEL,
	CAT_OP,
	COMMENT,
	ERROR,
	NEWLINE,
	QUIT,
	START,
	NSTATES
}				t_state;

# define NAME_MAX_LEN 128
# define DESC_MAX_LEN 2048

typedef struct	s_header
{
	unsigned int	magic;
	char			name[NAME_MAX_LEN + 1];
	unsigned short	size;
	char			desc[DESC_MAX_LEN + 1];
}				t_header;

typedef struct	s_line
{
	char			*data;
	int				col_num;
	int				line_num;
	struct s_line	*next;
}				t_line;

typedef struct	s_token
{
	char			*data;
	t_symbol		type;
	int				line_num;
	int				col_num;
	struct s_token	*next;
}				t_token;

typedef struct	s_parse
{
	t_header	header;
	t_line		*lines;
	t_token		*tokens;
	t_state		state;
	t_event		event;
}				t_parse;

/*
** load.c
*/

t_line			*load_file(char *file);
void			unload_lines(t_line *lines);

/*
** token.c
*/

void			destroy_tokens(t_token *tokens);
t_token			*tokenize(t_line *line, char *delim);

/*
** util.c
*/

void			fatal_error(char *msg);
unsigned int	reverse_bytes(unsigned int n, ssize_t size);
int				trim(char *arg);
#endif
