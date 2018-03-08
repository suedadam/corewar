/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 20:27:22 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 02:08:26 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"

const t_conv	g_disp[] = {
	{ '%', &print_percent },
	{ 'C', &print_char },
	{ 'c', &print_char },
	{ 'D', &print_ld_deprecated },
	{ 'd', &print_d },
	{ 'F', &print_float },
	{ 'f', &print_float },
	{ 'i', &print_d },
	{ 'O', &print_lo_deprecated },
	{ 'o', &print_o },
	{ 'p', &print_p },
	{ 'S', &print_str },
	{ 's', &print_str },
	{ 'U', &print_lu_deprecated },
	{ 'u', &print_u },
	{ 'X', &print_x },
	{ 'x', &print_x },
};
const int		g_disp_count = sizeof(g_disp) / sizeof(t_conv);

static int		print_line(const char *format, va_list *ap, t_bool *error)
{
	const char	*end;
	int			len;
	int			arg_len;

	len = 0;
	while (*format)
	{
		end = format;
		while (*end && *end != '%')
			end++;
		len += write(2, format, end - format);
		if (*end == '%')
		{
			end++;
			if ((arg_len = handle_arg(&end, ap, len)) < 0)
				*error = TRUE;
			else
				len += arg_len;
		}
		format = end;
	}
	return (len);
}

int				dispatch(t_arg *arg)
{
	t_conv	disp;
	char	*esc;
	int		out_len;
	int		i;

	out_len = 0;
	if (F(F_ESCAPE))
	{
		out_len += write(2, "\x1B[", 2);
		esc = va_arg(*arg->ap, char *);
		out_len += write(2, esc, ft_strlen(esc));
	}
	i = 0;
	while (i < g_disp_count)
	{
		disp = g_disp[i++];
		if (arg->conv == disp.specifier)
			out_len += (disp.handle(arg));
	}
	if (F(F_ESCAPE))
		out_len += write(2, "\x1b[0m", 4);
	return (out_len);
}

int				ft_printf(const char *format, ...)
{
	va_list	ap;
	int		len;
	t_bool	error;

	va_start(ap, format);
	error = FALSE;
	len = print_line(format, &ap, &error);
	va_end(ap);
	return ((error) ? -1 : len);
}

void			set_prefix(t_arg *arg, int base, char *num)
{
	char	conv;

	if (*arg->prefix)
		return ;
	if (F(F_HASH) && (arg->conv == 'p' || *num != '0'))
	{
		if (base == 16 || base == 8)
			arg->prefix[0] = '0';
		if (base == 16)
			arg->prefix[1] = ft_isupper(arg->conv) ? 'X' : 'x';
	}
	conv = ft_tolower(arg->conv);
	if (conv != 'd' && conv != 'i' && conv != 'f')
		return ;
	if (F(F_PLUS))
		*arg->prefix = '+';
	else if (F(F_SPACE))
		*arg->prefix = ' ';
}

int				write_pad(int size, char c)
{
	char	*pad;

	if (size < 1 || !(pad = (char *)malloc(size)))
		return (0);
	ft_memset(pad, c, size);
	write(2, pad, size);
	free(pad);
	return (size);
}
