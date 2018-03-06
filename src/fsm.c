/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:07:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/05 23:10:46 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

/*
** STATES             EVENTS
** ------             ------
** BUILD_DLABEL,      EV_COLON,
** BUILD_SYM,         EV_COMMENT,
** CAT_LABEL,         EV_DIRECT,
** CAT_OP,            EV_ALPHA,
** ERROR,             EV_NULL_TERM,
** QUIT,              EV_NUMBER,
** START              EV_PERIOD,
**                    EV_REGISTER,
**                    EV_WHITESP
*/

t_state (*g_trans[NSTATES][NEVENTS])(t_parse *parse) = {
	{
		
	}
}

char	*g_errmsg[NSTATES][NEVENTS] = {
	{

	}
}

t_state			fsm_error(t_parse *parse)
{
	UNUSED(parse);
	FATAL_ERROR(parse->err_msg);
	return (QUIT);
}

void			fsm_run(t_parse *parse)
{
	t_state	state;
	t_event	event;

	state = START;
	while (state != QUIT)
	{
		event = get_event(calc->base, *calc->pos);
		state = g_trans[state][event](calc);
		calc->pos++;
	}
}

static t_event	get_event(char c)
{
	if (!c)
		return (EV_NULL_TERM);
	else if (c == 'r')
		return (EV_REGISTER);
	else if (c == ':')
		return (EV_COLON);
	else if (c == '%')
		return (EV_DIRECT);
	else if (c == ' ' || c == '\n' || c == '\t')
		return (EV_WHITESP);
	else if (c == '.')
		return (EV_PERIOD);
	else if (c == '#')
		return (EV_COMMENT);
	else if ((unsigned int)(c - 'a') < 26)
		return (EV_ALPHA);
	else if ((unsigned int)(c - '0') < 10)
		return (EV_NUMBER);
}
