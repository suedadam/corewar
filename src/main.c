/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 20:50:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/04 22:23:35 by sgardner         ###   ########.fr       */
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

int			main(int ac, char **av)
{
	if (ac < 2)
		print_usage(av[0]);
	return (0);
}
