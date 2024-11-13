/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:43:56 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/13 09:17:28 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

#define FD 1

ssize_t	ft_write_base(unsigned long long num, int base, char *digits)
{
	char	buffer[42];
	int		i;
	ssize_t	count;

	i = 0;
	count = 0;
	if (num == 0)
		return (write(FD, "0", 1));
	while (num)
	{
		buffer[i++] = digits[num % base];
		num /= base;
	}
	while (--i >= 0)
		if (write(FD, &buffer[i], 1) != 1)
			return (-1);
	else
		count++;
	return (count);
}

ssize_t	ft_write_int(int num)
{
	ssize_t	count;

	count = 0;
	if (num == INT_MIN)
		return (write(FD, "-2147483648", 11));
	if (num < 0)
	{
		if (write(FD, "-", 1) != 1)
			return (-1);
		count++;
		num = -num;
	}
	return (count + ft_write_base(num, 10, "0123456789"));
}

ssize_t	ft_write_str(char *str)
{
	ssize_t	count;

	count = 0;
	if (!str)
		str = "(null)";
	while (*str)
	{
		if (write(FD, str++, 1) != 1)
			return (-1);
		count++;
	}
	return (count);
}

ssize_t	ft_input_check(const char *format, size_t *i, va_list args)
{
	if (format[*i + 1] == 's')
		return (ft_write_str(va_arg(args, char *)));
	if (format[*i + 1] == 'd')
		return (ft_write_int(va_arg(args, int)));
	if (format[*i + 1] == 'x')
		return (ft_write_base(va_arg(args, unsigned int)
				, 16, "0123456789abcdef"));
	if (format[*i + 1] == '%')
		return (write(FD, "%", 1));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	ssize_t	count;
	ssize_t	res;
	size_t	i;

	i = 0;
	count = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] != '%')
			res = write(FD, &format[i], 1);
		else
		{
			res = ft_input_check(format, &i, args);
			i++;
		}
		if (res < 0)
			return (-1);
		count += res;
		i++;
	}
	va_end(args);
	return (count);
}

/*#include <stdio.h>
int	main(void)
{
	ft_printf("-ft_printf: %s\n", "toto");
	printf("+printf: %s\n", "toto");
	ft_printf("-ft_printf: Magic %s is %d\n", "number", 42);
	printf("+printf: Magic %s is %d\n", "number", 42);
	ft_printf("-ft_printf: Hexadecimal for %d is %x\n", 42, 42);
	printf("+printf: Hexadecimal for %d is %x\n", 42, 42);
	ft_printf("-ft_printf: INT_MAX test:%d\n", INT_MAX);
	printf("+printf: INT_MAX test:%d\n", INT_MAX);
	ft_printf("-ft_printf: INT_MIN test:%d\n", INT_MIN);
	printf("+printf: INT_MIN test:%d\n", INT_MIN);
}*/
