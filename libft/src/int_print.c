/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/07 14:28:28 by sgardner          #+#    #+#             */
/*   Updated: 2017/10/24 20:22:30 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_bool	fill_precision(t_arg *arg, char **num)
{
	char	*filled;
	int		len;

	arg->precision -= ft_tolower(arg->conv) == 'o' && (*arg->prefix == '0');
	if (!F(F_PRECISE) || (len = ft_strlen(*num)) >= arg->precision)
		return (TRUE);
	if (!(filled = (char *)malloc(arg->precision + 1)))
		return (FALSE);
	ft_memset((void *)filled, '0', arg->precision - len);
	ft_memcpy((void *)(filled + (arg->precision - len)), (void *)*num, len);
	filled[arg->precision] = '\0';
	free(*num);
	*num = filled;
	return (TRUE);
}

static char		*build_num(t_arg *arg, int base, t_bool is_signed)
{
	char		*num;
	intmax_t	n;
	uintmax_t	un;

	if (is_signed)
	{
		n = get_int(arg);
		if (!n && F(F_PRECISE) && !arg->precision)
			return (ft_strdup(""));
		num = pf_itoa(arg, n, base);
	}
	else
	{
		un = get_uint(arg);
		if (!un && F(F_PRECISE) && !arg->precision
				&& (!F(F_HASH) || ft_tolower(arg->conv) == 'x'))
			return (ft_strdup(""));
		num = pf_uitoa(un, base);
	}
	if (!num)
		return (NULL);
	set_prefix(arg, base, num);
	if (!fill_precision(arg, &num))
		return (NULL);
	return (num);
}

int				print_int(t_arg *arg, int base, t_bool is_signed)
{
	char	*num;
	int		len;
	int		prefix_len;
	int		pad;
	int		out_len;

	if (!(num = build_num(arg, base, is_signed)))
		return (-1);
	len = ft_strlen(num);
	prefix_len = ft_strlen(arg->prefix);
	out_len = 0;
	pad = arg->width - len - prefix_len;
	if (!F(F_MINUS) && (!F(F_ZERO) || F(F_PRECISE)))
		out_len += write_pad(pad, ' ');
	out_len += write(1, arg->prefix, prefix_len);
	if (F(F_ZERO) && !F((F_MINUS | F_PRECISE)))
		out_len += write_pad(pad, '0');
	if (arg->conv == 'X')
		ft_strupcase(num);
	out_len += write(1, num, len);
	if (F(F_MINUS))
		out_len += write_pad(pad, ' ');
	free(num);
	return (out_len);
}
