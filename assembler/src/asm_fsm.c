/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_fsm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:07:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/12 00:53:04 by sgardner         ###   ########.fr       */
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
	parse->curr = parse->tokens;
	while (parse->curr->type == SYM_COMMAND_NAME
		|| parse->curr->type == SYM_COMMAND_COMMENT
		|| parse->curr->type == UNDEFINED)
		parse->curr = parse->curr->next;
}

t_state			fsm_syntax_error(t_parse *parse)
{
	static char		*symnames[NSYMBOLS] = {
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

	syntax_error(
		parse->curr->row,
		parse->curr->col,
		symnames[parse->curr->type],
		parse->curr->data);
	return (QUIT);
}
