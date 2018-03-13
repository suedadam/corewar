/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_handlers_deprecated.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/09 18:46:50 by sgardner          #+#    #+#             */
/*   Updated: 2017/10/15 16:56:39 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_ld_deprecated(t_arg *arg)
{
	arg->flags = ((arg->flags >> 6) << 6) | F_L;
	return (print_int(arg, 10, TRUE));
}

int	print_lo_deprecated(t_arg *arg)
{
	arg->flags = ((arg->flags >> 6) << 6) | F_L;
	return (print_int(arg, 8, FALSE));
}

int	print_lu_deprecated(t_arg *arg)
{
	arg->flags = ((arg->flags >> 6) << 6) | F_L;
	return (print_int(arg, 10, FALSE));
}
