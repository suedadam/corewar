/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_structs.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/10 23:49:54 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 00:27:15 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_STRUCTS_H
# define CORE_STRUCTS_H

typedef struct		s_op
{
	char			*name;
	int				id;
	int				num_args;
	int				allowed[3];
	t_bool			coding;
	t_bool			truncate;
}					t_op;

typedef struct		s_header
{
	unsigned int	magic;
	char			name[PROG_NAME_LENGTH + 1];
	unsigned int	size;
	char			desc[COMMENT_LENGTH + 1];
}					t_header;

typedef struct		s_line
{
	char			*data;
	int				col;
	int				row;
	struct s_line	*next;
}					t_line;

typedef struct		s_token
{
	void			*data;
	t_symbol		type;
	int				cbyte;
	int				len;
	int				row;
	int				col;
	struct s_token	*next;
}					t_token;

typedef struct		s_parse
{
	t_state			state;
	t_event			event;
	t_header		header;
	t_token			*curr;
	t_token			*tokens;
	t_line			*lines;
}					t_parse;
#endif
