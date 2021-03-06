/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:11:42 by asyed             #+#    #+#             */
/*   Updated: 2018/03/18 16:13:21 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "vm.h"

void		dump_memory(t_byte *arena)
{
	int	i;
	int c;

	i = 0;
	c = 0;
	while (i < MEM_SIZE)
	{
		if (c == 32)
		{
			ft_printf("\n");
			c = 0;
		}
		ft_printf("%02x ", arena[i]);
		c++;
		i++;
	}
	ft_printf("\n");
}

void		write_memory(void *arena, t_byte *src, int offset, int size)
{
	offset = (offset + MEM_SIZE) % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((t_byte *)arena + offset++) = *src++;
	}
}

void		copy_memory_fwd_off(void *dst, t_byte *src, int offset, int size)
{
	offset = (offset + MEM_SIZE) % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((t_byte *)dst++) = *(src + offset++);
	}
}

uint32_t	ft_longswap(uint32_t const byte)
{
	uint8_t data[4];

	ft_memcpy(&data, &byte, sizeof(data));
	return (((uint32_t)data[3] << 0)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[0] << 24));
}

uint16_t	ft_shortswap(uint16_t const byte)
{
	uint8_t data[2];

	ft_memcpy(&data, &byte, sizeof(data));
	return (((uint32_t)data[1] << 0)
		| ((uint32_t)data[0] << 8));
}
