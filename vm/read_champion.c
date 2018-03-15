/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_champion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:21:01 by asyed             #+#    #+#             */
/*   Updated: 2018/03/15 14:11:33 by sgardner         ###   ########.fr       */
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
	if (size - (off_t)sizeof(t_header) < 0)
	{
		ft_printf("Invalid input file\n");
		return (-1);
	}
	if (size > (CHAMP_MAX_SIZE + (off_t)sizeof(t_header)))
	{
		ft_printf("File has too large a code (%lld bytes > %lld bytes)\n",
			size, CHAMP_MAX_SIZE + (off_t)sizeof(t_header));
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
	t_header	*player_h;

	player_h = (t_header *)player;
	if (player_h->magic != ft_longswap(COREWAR_EXEC_MAGIC))
		return (-1);
	if (ft_longswap(player_h->prog_size) != (size - sizeof(t_header)))
	{
		ft_printf("Error: Checksum error (%d bytes != %d bytes)\n",
			ft_longswap(player_h->prog_size), (size - sizeof(t_header)));
		return (-1);
	}
	return (0);
}

static int		load_to_mem(int fd, size_t size, void *arena, int player_id)
{
	void		*player;
	size_t		placement;
	t_process	*child;

	if (!(player = ft_memalloc(size + 1)) ||
		read(fd, player, size) == -1 || validate_header(player, size))
		return (-1);
	size -= sizeof(t_header);
	player += sizeof(t_header);
	placement = ((-player_id - 1) * (MEM_SIZE / g_taskmanager->total_players));
	ft_memcpy(arena + placement, player, size);
	free(player - sizeof(t_header));
	child = ft_memalloc(sizeof(t_process));
	child->next = g_taskmanager->processes;
	child->plid = player_id;
	child->regs[1] = player_id;
	child->pc = placement;
	g_taskmanager->processes = child;
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
