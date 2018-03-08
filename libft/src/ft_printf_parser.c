/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/04 12:01:33 by sgardner          #+#    #+#             */
/*   Updated: 2017/10/24 11:43:14 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	find_flags(const char *fmt, t_arg *arg)
{
	int	n;

	n = 0;
	while (fmt[n])
	{
		if (fmt[n] == '-')
			arg->flags |= F_MINUS;
		else if (fmt[n] == '+')
			arg->flags |= F_PLUS;
		else if (fmt[n] == ' ')
			arg->flags |= F_SPACE;
		else if (fmt[n] == '0')
			arg->flags |= F_ZERO;
		else if (fmt[n] == '#')
			arg->flags |= F_HASH;
		else if (fmt[n] == '&')
			arg->flags |= F_ESCAPE;
		else if (fmt[n] == '^')
			arg->flags |= F_UTF;
		else
			break ;
		n++;
	}
	return (n);
}

static int	find_length(const char *fmt, t_arg *arg)
{
	if (*fmt == 'h' && *(fmt + 1) == 'h')
	{
		arg->flags |= F_HH;
		return (2);
	}
	else if (*fmt == 'l' && *(fmt + 1) == 'l')
	{
		arg->flags |= F_LL;
		return (2);
	}
	else if (*fmt == 'h')
		arg->flags |= F_H;
	else if (*fmt == 'L')
		arg->flags |= F_LD;
	else if (*fmt == 'l')
		arg->flags |= F_L;
	else if (*fmt == 'j')
		arg->flags |= F_J;
	else if (*fmt == 'z')
		arg->flags |= F_Z;
	else
		return (0);
	return (1);
}

static int	find_precision(const char *fmt, t_arg *arg)
{
	int	n;

	if (*fmt != '.')
		return (0);
	arg->flags |= F_PRECISE;
	fmt++;
	if (*fmt == '*')
	{
		if ((arg->precision = va_arg(*arg->ap, int)) < 0)
			arg->flags ^= F_PRECISE;
		return (2);
	}
	n = 0;
	while (fmt[n] && ft_isdigit(fmt[n]))
		arg->precision = (arg->precision * 10) + (fmt[n++] - '0');
	return (1 + n);
}

static int	find_width(const char *fmt, t_arg *arg)
{
	int	n;

	if (*fmt == '*')
	{
		if ((arg->width = va_arg(*arg->ap, int)) < 0)
		{
			arg->flags |= F_MINUS;
			arg->width *= -1;
		}
		return (1);
	}
	n = 0;
	while (fmt[n] && ft_isdigit(fmt[n]))
		arg->width = (arg->width * 10) + (fmt[n++] - '0');
	return (n);
}

int			handle_arg(const char **fmt, va_list *ap, int len)
{
	static t_arg	arg;
	int				*n;

	ft_memset((void *)&arg, 0, sizeof(arg));
	arg.ap = ap;
	*fmt += find_flags(*fmt, &arg);
	*fmt += find_width(*fmt, &arg);
	*fmt += find_precision(*fmt, &arg);
	*fmt += find_length(*fmt, &arg);
	arg.conv = **fmt;
	(*fmt)++;
	if (arg.conv == 'n')
	{
		n = va_arg(*ap, int *);
		*n = len;
		return (0);
	}
	return (dispatch(&arg));
}
