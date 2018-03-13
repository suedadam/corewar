/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_champion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:21:01 by asyed             #+#    #+#             */
/*   Updated: 2018/03/13 15:17:35 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "ft_printf.h"
#include "vm.h"

static int		size_check(int fd)
{
	off_t	start;
	off_t	size;

	start = lseek(fd, 0, SEEK_CUR);
	size = lseek(fd, 0, SEEK_END) - start;
	if (size - (off_t)sizeof(header_t) < 0)
	{
		ft_printf("Invalid input file\n");
		return (-1);
	}
	if (size > CHAMP_MAX_SIZE + (off_t)sizeof(header_t))
	{
		printf("File has too large a code (%lld bytes > %lld bytes)\n",
			size, CHAMP_MAX_SIZE + (off_t)sizeof(header_t));
		return (-1);
	}
	if (lseek(fd, start, SEEK_SET) == -1)
	{
		ft_printf("Failed to calculate size (skip failure)\n");
		return (-1);
	}
	return (size);
}

static int		validate_header(void *player, size_t size)
{
	header_t	*player_h;

	player_h = (header_t *)player;
	if (player_h->magic != ft_longswap(COREWAR_EXEC_MAGIC))
		return (-1);
	if (ft_longswap(player_h->prog_size) != size - sizeof(header_t))
	{
		ft_printf("Error: Too large a code (%d bytes > %d bytes)\n",
			ft_longswap(player_h->prog_size), (int)(size - sizeof(header_t)));
		return (-1);
	}
	return (0);
}

static void		*link_last(void)
{
	t_process	*child;

	if (!(child = (g_taskmanager->processes)))
	{
		child = ft_memalloc(sizeof(t_process));
		g_taskmanager->processes = child;
	}
	else
	{
		while (child && child->next)
			child = child->next;
		child->next = ft_memalloc(sizeof(t_process));
	}
	return (child);
}

static int		load_to_mem(int fd, size_t size, void *arena, int player_id)
{
	void		*player;
	size_t		placement;
	t_process	*child;

	if (!(player = ft_memalloc(size + 1)) ||
		read(fd, player, size) == -1 || validate_header(player, size))
		return (-1);
	size -= sizeof(header_t);
	player += sizeof(header_t);
	placement = (player_id * (MEM_SIZE / g_taskmanager->totalPlayers));
	ft_memcpy(arena + placement, player, size);
	free(player - sizeof(header_t));
	child = link_last();
	child->plid = player_id;
	child->regs[1] = player_id;
	child->pc = placement;
	return (0);
}

int				read_champion(char *filename, void *arena, int player_id)
{
	int		fd;
	off_t	size;

	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		ft_printf("Failed to open file\n");
		return (-1);
	}
	if ((size = size_check(fd)) == -1)
	{
		close(fd);
		return (-1);
	}
	if (load_to_mem(fd, size, arena, player_id) == -1)
		return (-1);
	return (0);
}
