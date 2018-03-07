/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 20:50:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/07 12:59:30 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "core.h"

static void	print_usage(char *prog_name)
{
	write(2, "usage: ", 7);
	write(2, prog_name, ft_strlen(prog_name));
	write(2, " source_file ...\n", 17);
	exit(1);
}

static void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("%02d:%02d %s\n", tokens->line_num, tokens->col_num,
			tokens->data);
		tokens = tokens->next;
	}
}

static void	init_parse(t_parse *parse, char *file)
{
	parse->header.magic = reverse_bytes(0x00EA83F3, sizeof(int));
	parse->lines = load_file(file);
	parse->tokens = tokenize(parse->lines, ", \t");
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
		print_tokens(parse.tokens);
		destroy_tokens(parse.tokens);
		unload_lines(parse.lines);
	}
	return (0);
}
