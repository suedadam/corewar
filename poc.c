/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suedadam <suedadam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 14:12:47 by suedadam          #+#    #+#             */
/*   Updated: 2018/02/18 14:13:11 by suedadam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <limits.h>
#define PROG_NAME_LENGTH (128)
#define COMMENT_LENGTH (2048)
#define COREWAR_EXEC_MAGIC 0xea83f3

typedef struct header_s
{
  unsigned int magic;
  char prog_name [PROG_NAME_LENGTH + 1];
  unsigned int prog_size;
  char comment[COMMENT_LENGTH + 1];
} header_t;

typedef struct champion_s
{
	header_t	header;
	char		*instructions;
}				champion_t;

int	check_header(champion_t champ, size_t size)
{
	if (champ.header.magic != ntohl(COREWAR_EXEC_MAGIC))
	{
		printf("Failed to check header magic.\n");
		printf("Checksum = \"%x\"\n", champ.header.magic);
		return (-1);
	}
	printf("Program name = \"%s\"\n", champ.header.prog_name);
	printf("Comment = \"%s\"\n", champ.header.comment);
	if (htonl(champ.header.prog_size) != size - sizeof(header_t))
	{
		printf("Size = %u != %lu\n", htonl(champ.header.prog_size), size - sizeof(header_t));
		return (-1);
	}
	return (0);
}

int	main(void)
{
	size_t		lol;
	int			fd;
	FILE		*fp;
	size_t		size;
	void		*mapspace;
	champion_t	champ;

	fd = open("test.cor", O_RDWR);
	fp = fdopen(fd, "rw");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	mapspace = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	memcpy(&champ.header, mapspace, sizeof(header_t));
	champ.instructions = mapspace + sizeof(header_t);
	// memcpy(mapspace, &(champ->header), sizeof(header_t));
	if (size > INT_MAX)
	{
		printf("File too large %zu\n", size);
		return (-1);
	}
	if (check_header(champ, size))
	{
		printf("Failed check_header()\n");
		return (-1);
	}
	if (munmap(mapspace, size))
	{
		printf("Failed to close\n");
		return (-1);
	}
	printf("Passed checks :)\n");
	return (0);
}

