/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_champion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:21:01 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 16:16:38 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		printf("too large? %lld > %d\n", size-start, CHAMP_MAX_SIZE);
		return (-1);
	}
	if (lseek(fd, start, SEEK_SET) == -1)
	{
		printf("Failed to set it to the start\n");
		return (-1);
	}
	return (size - start);
}

static int	validate_header(void *player, size_t size)
{
	header_t	*playerHeader;

	playerHeader = (header_t *)player;
	if (playerHeader->magic != ntohl(COREWAR_EXEC_MAGIC))
		return (-1);
	if (ntohl(playerHeader->prog_size) != size - sizeof(header_t))
		return (-1);
	return (0);
}

/*
** I always like to pad the end of my shit with a byte ;P 
*/

static int	load_to_mem(int fd, size_t size, void *arena, int playerID)
{
	void	*player;
	size_t	placement;

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
	if (validate_header(player, size))
	{
		printf("Header mismatch!\n");
		return (-1);
	}
	size -= sizeof(header_t);
	player += sizeof(header_t);
	printf("totalPlayers = %d\n", taskmanager->totalPlayers);
	placement = (playerID * (MEM_SIZE / taskmanager->totalPlayers));
	printf("[ID: %d] offset = %zu size = %zu\n", playerID, placement, size);
	if (placement + size > MEM_SIZE)
	{
		printf("woa there big boi ;)\n");
		exit(1);
	}
	ft_memcpy(arena + placement, player, size);
	free(player - sizeof(header_t));
	if (!(((taskmanager->players)[playerID])->processes = ft_memalloc(sizeof(t_process))))
		return (-1);
	(taskmanager->players)[playerID]->processes->regs[1] = playerID;
	((taskmanager->players)[playerID])->processes->pc = placement;
	return (0);
}

int			read_champion(char *filename, void *arena, int playerID)
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
	if (load_to_mem(fd, size, arena, playerID) == -1)
		return (-1);
	return (0);
}
