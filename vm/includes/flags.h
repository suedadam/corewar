/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 22:22:05 by asyed             #+#    #+#             */
/*   Updated: 2018/03/13 13:16:04 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FLAGS_H
# define FT_FLAGS_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct	s_flagdefine
{
	char		idchar;
	uint8_t		required:1;
	uint8_t		caps:1;
	uint8_t		valafter:1;
	char		*desc;
	void		*result;
}				t_flagdefine;

t_flagdefine g_flags[] = {
	{'D', 0, 1, 1, "Just another flag", NULL},
	{0, 0, 0, 0, NULL, NULL},
};

#endif
