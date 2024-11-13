/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:14:24 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/13 09:50:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(len1 + len2 + 1);
	i = 0;
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (*s2)
		str[i++] = *s2++;
	str[i] = '\0';
	free(s1);
	return (str);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	slen;
	size_t	i;
	char	*res;

	slen = ft_strlen(s);
	if (start >= slen)
	{
		res = malloc(1);
		if (res)
			res[0] = '\0';
		return (res);
	}
	if (slen - start < len)
		len = slen - start;
	res = (char *)malloc(len + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < len)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}

char	*new_stash(char *stash)
{
	char	*ptr;
	char	*nstash;

	ptr = ft_strchr(stash, '\n');
	if (!ptr)
	{
		free(stash);
		return (NULL);
	}
	nstash = ft_substr(stash, (ptr - stash) + 1, ft_strlen(stash)
			- (ptr - stash) - 1);
	free(stash);
	return (nstash);
}

char	*make_line(char *stash)
{
	char	*ptr;

	ptr = ft_strchr(stash, '\n');
	if (!ptr)
		return (ft_substr(stash, 0, ft_strlen(stash)));
	return (ft_substr(stash, 0, (ptr - stash) + 1));
}

char	*read_buf(int fd, char *stash)
{
	char	buffer[BUFFER_SIZE + 1];
	int		rb;

	rb = read(fd, buffer, BUFFER_SIZE);
	while (rb > 0)
	{
		buffer[rb] = '\0';
		stash = ft_strjoin(stash, buffer);
		if (stash == NULL)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
		rb = read(fd, buffer, BUFFER_SIZE);
	}
	if (rb == -1)
	{
		free(stash);
		return (NULL);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (!stash || !ft_strchr(stash, '\n'))
		stash = read_buf(fd, stash);
	if (!stash)
		return (NULL);
	line = make_line(stash);
	stash = new_stash(stash);
	return (line);
}

/*#include <stdio.h>
int	main(void)
{
	int fd = open("test.txt", 0);
	char	*line = get_next_line(fd);
	while (line)
	{
		printf("%s\n", line);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	return (0);
}*/
