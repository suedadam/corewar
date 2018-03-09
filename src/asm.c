/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 20:50:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/09 04:13:09 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "ft_printf.h"

static void	print_usage(char *prog_name)
{
	write(2, "usage: ", 7);
	write(2, prog_name, ft_strlen(prog_name));
	write(2, " source_file ...\n", 17);
	exit(1);
}

static void	print_debug(t_parse *parse)
{
	t_token	*tokens;

	ft_printf("Name: %s\nDesc: %s\n", parse->header.name, parse->header.desc);
	ft_printf("-----------------------------------\n");
	tokens = parse->tokens;
	while (tokens)
	{
		ft_printf("%02d:%02d (%02d) ", tokens->line_num, tokens->col_num,
			tokens->len, tokens->data);
		if (tokens->type == SYM_REGISTER)
			ft_printf("R%d\n", *tokens->data);
		else if (tokens->type == SYM_IND)
			ft_printf("%hd\n", *tokens->data);
		else if (tokens->type == SYM_DIRECT)
			ft_printf("%%%d\n", *tokens->data);
		else if (tokens->type == SYM_OP)
			ft_printf("%s\n", g_ops[*(int *)tokens->data - 1].name);
		else
			ft_printf("%s\n", tokens->data);
		tokens = tokens->next;
	}
	ft_printf("-----------------------------------\n");
}

static void	init_parse(t_parse *parse, char *file)
{
	static char	delim[4] = { SEPARATOR_CHAR, ' ', '\t', '\0' };

	ft_memset(&parse->header, 0, sizeof(t_header));
	parse->header.magic = reverse_bytes(COREWAR_EXEC_MAGIC, sizeof(int));
	parse->lines = load_file(file);
	parse->tokens = tokenize(parse->lines, delim);
}

static char	*build_name(char *file)
{
	char	*new;
	char	*end;
	int		len;

	end = ft_strrchr(file, '.');
	len = (end) ? (end - file) : ft_strlen(file);
	if (!(new = ft_memalloc(len + 5)))
		DEFAULT_ERROR;
	end = ft_stpncpy(new, file, len);
	ft_stpcpy(end, ".cor");
	return (new);
}

static void	write_file(t_parse *parse, char *file)
{
	int	fd;

	if ((fd = open(file, O_WRONLY | O_CREAT, 0600)) < 0)
		DEFAULT_ERROR;
	write(fd, &parse->header, sizeof(t_header));
	close(fd);
	free(file);
}

int			main(int ac, char **av)
{
	t_parse	parse;
	int		i;

	if (ac < 2)
		print_usage(av[0]);
	i = 0;
	while (++i < ac)
	{
		init_parse(&parse, av[i]);
		fsm_run(&parse);
		print_debug(&parse);
		write_file(&parse, build_name(av[i]));
		destroy_tokens(parse.tokens);
		unload_lines(parse.lines);
	}
	return (0);
}
