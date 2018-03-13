/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 22:38:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/12 00:53:52 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"

static int	label_pointer(t_parse *parse, int pos)
{
	static int	ptr;
	t_token		*iter;

	ptr = 0;
	iter = parse->tokens;
	while (iter)
	{
		if (iter->type == SYM_LABEL
			&& !ft_strcmp(parse->curr->data, iter->data))
			break ;
		iter = iter->next;
	}
	ptr = iter->cbyte - pos;
	return (ptr);
}

static void	write_token(t_parse *parse, int pos, int fd)
{
	t_token	*curr;
	int		data;

	curr = parse->curr;
	if (curr->type == SYM_OP)
	{
		write(fd, curr->data, 1);
		if (curr->len > 1)
			write(fd, &curr->cbyte, 1);
		return ;
	}
	data = (curr->type == SYM_DLABEL || curr->type == SYM_INDLABEL) ?
		label_pointer(parse, pos) : *(int *)curr->data;
	if (curr->len > 1)
		data = reverse_bytes(data, curr->len);
	write(fd, &data, curr->len);
}

void		write_file(t_parse *parse, char *file)
{
	int	fd;
	int	pos;
	int	op_pos;

	pos = 0;
	op_pos = 0;
	if ((fd = open(file, O_WRONLY | O_CREAT, 0600)) < 0)
		DEFAULT_ERROR;
	parse->header.size = reverse_bytes(parse->header.size,
		sizeof(parse->header.size));
	write(fd, &parse->header, sizeof(t_header));
	while (parse->curr)
	{
		if (parse->curr->type != SYM_LABEL)
		{
			write_token(parse, op_pos, fd);
			if (parse->curr->type == SYM_OP)
				op_pos = pos;
			pos += parse->curr->len;
		}
		parse->curr = parse->curr->next;
	}
	close(fd);
	free(file);
}
