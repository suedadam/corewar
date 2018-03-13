/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 22:13:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/12 00:38:09 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

#define ERR_EOF(x) syntax_error(x->row, x->col + x->len, "ENDLINE", "")
#define CPTR(x) (char *)x->data

static char	*build_val(t_token *start, t_token *end, int len)
{
	char	*val;
	char	*tmp;
	t_token	*curr;

	if (!(val = ft_memalloc(len + 1)))
		DEFAULT_ERROR;
	tmp = val;
	curr = start;
	while (TRUE)
	{
		tmp = ft_stpcpy(tmp, curr->data);
		if (curr == end)
			return (val);
		else
			*tmp++ = ' ';
		curr = curr->next;
	}
}

static int	command_len(t_token *start, t_token **end)
{
	t_token	*curr;
	char	*pos;
	int		len;

	len = 0;
	curr = start;
	while ((curr = curr->next))
	{
		len += curr->len + 1;
		if (!curr->next)
			ERR_EOF(curr);
		if ((pos = ft_strchr(curr->data, '\"')))
		{
			if (pos != CPTR(curr) + curr->len - 1)
				lexical_error(curr->row, curr->col + (pos - CPTR(curr)) + 1);
			--len;
			break ;
		}
	}
	*end = curr;
	return (len);
}

static void	oversized(char *type, int max_len)
{
	ft_printf("%&s Champion %s too long (Max length %d)\n", "1;31m", "ERROR:",
		type, max_len);
	exit(1);
}

static char	*get_val(t_parse *parse, char *type, int max_len)
{
	t_token	*start;
	t_token	*end;
	char	*val;
	int		len;

	start = parse->curr;
	len = start->len;
	end = start;
	if (*(char *)start->data != '\"')
		lexical_error(start->row, start->col);
	if (ft_strrchr(start->data, '\"') == start->data)
		++len;
	if (len > start->len)
		len += command_len(start, &end);
	if (len - 2 > max_len)
		oversized(type, max_len);
	val = build_val(start, end, len);
	if (val[len - 1] != '\"')
		lexical_error(end->row, end->col + (ft_strrchr(val, '\"') - val) + 1);
	trim(val, " \t\n\"");
	parse->curr = end;
	return (val);
}

t_state		fsm_build_header(t_parse *parse)
{
	char	*data;
	char	*val;
	int		max_len;

	if (parse->curr->type == SYM_COMMAND_NAME)
	{
		data = parse->header.name;
		max_len = PROG_NAME_LENGTH;
	}
	else
	{
		data = parse->header.desc;
		max_len = COMMENT_LENGTH;
	}
	if (*data)
		fsm_syntax_error(parse);
	if (!parse->curr->next)
		ERR_EOF(parse->curr);
	parse->curr = parse->curr->next;
	val = get_val(parse, (data == parse->header.name) ? "name" : "comment",
		max_len);
	ft_stpcpy(data, val);
	free(val);
	return (BUILD_HEADER);
}
