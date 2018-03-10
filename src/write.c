/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 22:38:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 14:13:32 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "core.h"

void	write_file(t_parse *parse, char *file)
{
	int	fd;

	if ((fd = open(file, O_WRONLY | O_CREAT, 0600)) < 0)
		DEFAULT_ERROR;
	write(fd, &parse->header, sizeof(t_header));
	close(fd);
}
