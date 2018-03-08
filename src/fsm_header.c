/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 22:13:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/07 23:07:03 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

t_state	fsm_build_header(t_parse *parse)
{
	UNUSED(parse);
	return (BUILD_HEADER);
}
