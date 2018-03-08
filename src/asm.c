/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 20:50:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 04:37:11 by sgardner         ###   ########.fr       */
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

static void	print_debug(t_parse *parse)
{
	t_token	*tokens;

	printf("Name: %s\nDesc: %s\n", parse->header.name, parse->header.desc);
	printf("-----------------------------------\n");
	tokens = parse->tokens;
	while (tokens)
	{
		printf("%02d:%02d (%02d) %s\n", tokens->line_num, tokens->col_num,
			tokens->len, tokens->data);
		tokens = tokens->next;
	}
	printf("-----------------------------------\n");
}

static void	init_parse(t_parse *parse, char *file)
{
	static char	delim[4] = { SEPARATOR_CHAR, ' ', '\t', '\0' };

	parse->header.magic = reverse_bytes(COREWAR_EXEC_MAGIC, sizeof(int));
	parse->lines = load_file(file);
	parse->tokens = tokenize(parse->lines, delim);
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
		destroy_tokens(parse.tokens);
		unload_lines(parse.lines);
	}
	return (0);
}
