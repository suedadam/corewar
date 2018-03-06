/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:06:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/05 19:17:46 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

static t_token	create_token(t_symbol type)
{
	t_token	*tok;

	if (!(tok = ft_memalloc(sizeof(t_token))))
		DEFAULT_ERROR;
	tok->type = type;
	return (tok);
}

void			add_token(t_parse *parser, t_symbol type)
{
	t_token *tok;

	tok = create_token(type);
	if (parser->end)
		parser->end->next = tok;
	else
		parser->start = tok;
	parser->end = tok;
}
