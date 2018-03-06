/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_champion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:21:01 by asyed             #+#    #+#             */
/*   Updated: 2018/03/05 19:45:46 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "vm.h"
#include <fcntl.h>

static int	size_check(int fd)
{
	off_t	size;
	off_t	start;

	start = lseek(fd, 0, SEEK_CUR);
	size = lseek(fd, 0, SEEK_END);
	if ((size - start) > CHAMP_MAX_SIZE) //Cast to off_t?
	{
		printf("too large? %d > %d\n", size-start, CHAMP_MAX_SIZE);
		return (-1);
	}
	if (lseek(fd, start, SEEK_SET) == -1)
	{
		printf("Failed to set it to the start\n");
		return (-1);
	}
	return (size - start);
}

static int	load_to_mem(int fd, size_t size)
{
	void	*player;

	if (!(player = ft_memalloc(size + 1)))
	{
		printf("Failed to memalloc(%s)\n", strerror(errno));
		return (-1);
	}
	if (read(fd, player, size) == -1)
	{
		printf("Failed to read into buf? \"%s\"\n", strerror(errno));
		free(player);
		return (-1);
	}
	return (0);
}

int			read_champion(char *filename, void *arena)
{
	int		fd;
	off_t	size;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (-1);
	if ((size = size_check(fd)) == -1 || size > ((int)(1<<31)) - 1)
	{
		printf("%lld > %d\n", size, ((int)(1<<31) - 1));
		close(fd);
		return (-1);
	}
	printf("Was okay in size :tada:\n");
	if (load_to_mem(fd, size) == -1)
		return (-1);
	return (0);
}
