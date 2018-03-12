/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:11:42 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 12:58:24 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	write_memory(void *arena, unsigned char *src, int offset, int size)
{
	offset = (offset + MEM_SIZE) % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)arena + offset++) = *src++;
	}
}

void	copy_memory_fwd_off(void *dst, unsigned char *src, int offset, int size)
{
	offset = (offset + MEM_SIZE) % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)dst++) = *(src + offset++);
	}
}

uint32_t ft_longswap(uint32_t const byte)
{
	uint8_t data[4];

	memcpy(&data, &byte, sizeof(data));
    return ((uint32_t) data[3] << 0)
         | ((uint32_t) data[2] << 8)
         | ((uint32_t) data[1] << 16)
         | ((uint32_t) data[0] << 24);
}

uint16_t ft_shortswap(uint16_t const byte)
{
	uint8_t data[2];

	memcpy(&data, &byte, sizeof(data));
    return ((uint32_t) data[3] << 0)
         | ((uint32_t) data[2] << 8);
}
