/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 20:50:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 20:44:19 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

static char	*build_filename(char *file)
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

static void	init_parse(t_parse *parse, char *file)
{
	static char	delim[4] = { SEPARATOR_CHAR, ' ', '\t', '\0' };

	ft_printf("Compiling %s...\n > ", file);
	ft_memset(&parse->header, 0, sizeof(t_header));
	parse->header.magic = reverse_bytes(COREWAR_EXEC_MAGIC, sizeof(int));
	parse->lines = load_file(file);
	parse->tokens = tokenize(parse->lines, delim);
}

static void	print_debug(t_parse *parse)
{
	t_token	*tokens;

	ft_printf("Name: %s\nDesc: %s\n", parse->header.name, parse->header.desc);
	ft_printf("Size: %d\n", parse->header.size);
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

static void	print_usage(char *prog_name)
{
	ft_printf("usage: %s source_file ...\n", prog_name);
	exit(1);
}

int			main(int ac, char **av)
{
	t_parse	parse;
	char	*ofile_name;
	int		i;

//	UNUSED(print_debug);
	if (ac < 2)
		print_usage(av[0]);
	i = 0;
	while (++i < ac)
	{
		init_parse(&parse, av[i]);
		fsm_run(&parse);
print_debug(&parse);
		ofile_name = build_filename(av[i]);
		write_file(&parse, ofile_name);
		free(ofile_name);
		destroy_tokens(parse.tokens);
		unload_lines(parse.lines);
		ft_printf("%&s", "1;32m", "DONE\n");
	}
	return (0);
}
