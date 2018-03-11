/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coredef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/10 23:05:35 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 04:40:14 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREDEF_H
# define COREDEF_H

/*
** Header
*/

# define COREWAR_EXEC_MAGIC	0x00EA83F3
# define NAME_CMD_STRING	".name"
# define PROG_NAME_LENGTH	128
# define COMMENT_CMD_STRING	".comment"
# define COMMENT_LENGTH		2048

/*
** Keys
*/

# define COMMENT_CHAR		'#'
# define LABEL_CHAR			':'
# define DIRECT_CHAR		'%'
# define SEPARATOR_CHAR		','
# define LABEL_CHARS		"abcdefghijklmnopqrstuvwxyz_0123456789"

/*
** Direct Value
*/

# define T_D				2
# define DIR_CODE			2
# define DIR_SIZE			4

/*
** Indirect Value
*/

# define T_I				4
# define IND_CODE			3
# define IND_SIZE			2

/*
** Registry
*/

# define T_R				1
# define REG_CODE			1
# define REG_SIZE			1
# define REG_NUMBER			16

typedef enum		e_event
{
	EV_LABEL,
	EV_OP,
	EV_PERIOD,
	NEVENTS
}					t_event;

typedef enum		e_state
{
	BUILD_HEADER,
	BUILD_LABEL,
	BUILD_OP,
	QUIT,
	START,
	NSTATES
}					t_state;

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
#endif
