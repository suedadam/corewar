/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:07:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 21:35:22 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

/*
** STATES             EVENTS
** ------             ------
** BUILD_HEADER,      EV_LABEL,
** BUILD_LABEL,       EV_OP,
** BUILD_OP,          EV_PERIOD
** QUIT,
** START
*/

static t_state	(*g_trans[NSTATES][NEVENTS])(t_parse *parse) = {
	{
		fsm_build_label,
		fsm_build_op,
		fsm_build_header
	},
	{
		fsm_build_label,
		fsm_build_op,
		fsm_syntax_error
	},
	{
		fsm_build_label,
		fsm_build_op,
		fsm_syntax_error
	},
	{
		NULL,
		NULL,
		NULL
	},
	{
		fsm_syntax_error,
		fsm_syntax_error,
		fsm_build_header
	}
};

static char		*g_symnames[NSYMBOLS] = {
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
	if (!ft_strcmp(data, NAME_CMD_STRING))
	{
		tok->type = SYM_COMMAND_NAME;
		return (EV_PERIOD);
	}
	else if (!ft_strcmp(data, COMMENT_CMD_STRING))
	{
		tok->type = SYM_COMMAND_COMMENT;
		return (EV_PERIOD);
	}
	else if (data[tok->len - 1] == LABEL_CHAR)
	{
		data[--tok->len] = '\0';
		tok->type = SYM_LABEL;
		return (EV_LABEL);
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
	syntax_error(
		parse->curr->line_num,
		parse->curr->col_num,
		g_symnames[parse->curr->type],
		parse->curr->data);
	return (QUIT);
}
