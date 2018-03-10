/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 18:57:42 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/09 23:58:46 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "get_next_line.h"

static char		*strip_comments(char *arg)
{
	t_bool	comment;
	int		i;

	i = 0;
	comment = FALSE;
	while (arg[i])
	{
		if (comment)
			arg[i] = '\0';
		else if (arg[i] == COMMENT_CHAR)
			comment = TRUE;
		else
			++i;
	}
	return (arg);
}

static void		read_file(t_line **head, int fd)
{
	char	*line;
	int		line_num;
	int		res;

	line_num = 1;
	while ((res = get_next_line(fd, &line)) > 0)
	{
		if (!(*head = ft_memalloc(sizeof(t_line))))
			DEFAULT_ERROR;
		(*head)->data = strip_comments(line);
		(*head)->col_num = trim(line) + 1;
		(*head)->line_num = line_num++;
		if (*(*head)->data)
		{
			head = &(*head)->next;
			continue ;
		}
		free(line);
		ft_memdel((void **)head);
	}
	if (res < 0)
		DEFAULT_ERROR;
}

t_line			*load_file(char *file)
{
	t_line	*head;
	int		fd;

	if ((fd = open(file, O_RDONLY)) < 0)
		DEFAULT_ERROR;
	head = NULL;
	read_file(&head, fd);
	close(fd);
	if (!head)
		fatal_error("Empty or invalid input file");
	return (head);
}

void			unload_lines(t_line *lines)
{
	t_line	*next;

	while (lines)
	{
		free(lines->data);
		next = lines->next;
		free(lines);
		lines = next;
	}
}
