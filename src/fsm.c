/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:07:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 00:32:41 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

/*
** STATES             EVENTS
** ------             ------
** BUILD_HEADER,      EV_LABEL,
** BUILD_LABEL,       EV_OP,
** BUILD_OP,          EV_PERIOD
** QUIT,
** START
*/

t_state			(*g_trans[NSTATES][NEVENTS])(t_parse *parse) = {
	{ // BUILD_HEADER
		NULL,
		NULL,
		NULL
	},
	{ // BUILD_LABEL
		NULL,
		NULL,
		NULL
	},
	{  // BUILD_OP
		NULL,
		NULL,
		NULL
	},
	{ // QUIT
		NULL,
		NULL,
		NULL
	},
	{ // START
		fsm_syntax_error,
		fsm_syntax_error,
		fsm_build_header
	}
};

char			*g_symnames[NSYMBOLS] = {
	"COMMAND_NAME",
	"COMMAND_COMMENT",
	"DIRECT_PARAM",
	"DIRECT_LABEL",
	"INDIRECT_PARAM",
	"INDIRECT_LABEL",
	"LABEL",
	"INSTRUCTION",
	"REGISTER",
	"UNDEFINED"
};

static t_event	get_event(t_token *tok)
{
	char	*data;

	data = tok->data;
	if (data[tok->len - 1] == LABEL_CHAR)
	{
		tok->type = SYM_LABEL;
		return (EV_LABEL);
	}
	else if (!ft_strcmp(data, NAME_CMD_STRING))
	{
		tok->type = SYM_COMMAND_NAME;
		return (EV_PERIOD);
	}
	else if (!ft_strcmp(data, COMMENT_CMD_STRING))
	{
		tok->type = SYM_COMMAND_COMMENT;
		return (EV_PERIOD);
	}
	else
	{
		tok->type = SYM_OP;
		return (EV_OP);
	}
}

void			fsm_run(t_parse *parse)
{
	parse->state = START;
	parse->curr = parse->tokens;
	while (parse->curr)
	{
		parse->event = get_event(parse->curr);
		parse->state = g_trans[parse->state][parse->event](parse);
		parse->curr = parse->curr->next;
	}
}

t_state			fsm_syntax_error(t_parse *parse)
{
	t_token	*curr;

	curr = parse->curr;
	ft_printf("Syntax error at token [TOKEN][%03d:%03d] %s \"%s\"\n",
		curr->line_num, curr->col_num, g_symnames[curr->type], curr->data);
	exit(1);
	return (QUIT);
}
