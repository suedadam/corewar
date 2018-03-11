/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:06:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 14:50:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

void			destroy_tokens(t_token *tokens)
{
	t_token *next;

	while (tokens)
	{
		next = tokens->next;
		if (tokens->type == SYM_DIRECT
			|| tokens->type == SYM_IND
			|| tokens->type == SYM_REGISTER)
			free(tokens->data);
		free(tokens);
		tokens = next;
	}
}

static t_token	*create_token(char *data, t_symbol type)
{
	t_token	*tok;

	if (!(tok = ft_memalloc(sizeof(t_token))))
		DEFAULT_ERROR;
	tok->data = data;
	tok->type = type;
	return (tok);
}

static t_token	*split(char *data, char *delim, int row, int col)
{
	t_token	**curr;
	t_token	*head;
	int		i;

	i = 0;
	head = NULL;
	curr = &head;
	while (data[i])
	{
		while (ft_strchr(delim, data[i]))
			data[i++] = '\0';
		if (data[i])
		{
			*curr = create_token(data + i, UNDEFINED);
			(*curr)->row = row;
			(*curr)->col = col + trim((*curr)->data) + i;
			if (*(char *)(*curr)->data)
				curr = &(*curr)->next;
			else
				ft_memdel((void **)curr);
		}
		while (data[i] && !ft_strchr(delim, data[i]))
			++i;
	}
	return (head);
}

t_token			*tokenize(t_line *lines, char *delim)
{
	t_token **curr;
	t_token	*head;
	t_token	*tokens;

	head = NULL;
	curr = &head;
	while (lines)
	{
		if ((tokens = split(lines->data, delim, lines->row, lines->col)))
			*curr = tokens;
		while (*curr)
		{
			(*curr)->len = ft_strlen((*curr)->data);
			curr = &(*curr)->next;
		}
		lines = lines->next;
	}
	return (head);
}
