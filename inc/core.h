/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 19:02:54 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/05 22:13:30 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H
# include "libft.h"
# include <stdio.h> // REMOVE BEFORE SUBMISSION

# define FATAL_ERROR(msg) fatal_error(msg)
# define DEFAULT_ERROR FATAL_ERROR(strerror(errno))

typedef enum	e_event
{
	EV_COLON,
	EV_COMMENT,
	EV_DIRECT,
	EV_ALPHA,
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
	SYM_UNDEFINED,
	NSYMBOLS
}				t_symbol;

typedef enum	e_state
{
	BUILD_DLABEL,
	BUILD_SYM,
	CAT_LABEL,
	CAT_OP,
	ERROR,
	QUIT,
	START,
	NSTATES
}				t_state;

typedef struct	s_token
{
	t_symbol		type;
	char			*data;
	struct s_token	*next;
}				t_token;

typedef struct	s_parse
{
	t_header	header;
	t_token		*start;
	t_token		*end;
	char		*err_msg;
	int			line;
	int			col;
	char		prev;
}				t_parse;

# define NAME_MAX_LEN 128
# define DESC_MAX_LEN 2048

typedef struct	s_header
{
	unsigned int	magic;
	char			name[NAME_MAX_LEN + 1];
	unsigned short	size;
	char			desc[DESC_MAX_LEN + 1];
}				t_header;

/*
** util.c
*/

void			fatal_error(char *msg);
unsigned int	reverse_bytes(unsigned int n, ssize_t size);
#endif
