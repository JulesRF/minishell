/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 16:49:35 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/30 16:18:14 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*pop_line(char *s)
{
	int		i;
	char	*line;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
	{
		free(s);
		return (NULL);
	}
	ft_strlcpy(line, s, i + 2);
	free(s);
	return (line);
}

char	*retrieve_unused_buffer(char *s, char *buffer)
{
	int	buffer_len;

	ft_strlcpy(buffer, ft_strchr(buffer, '\n') + 1, BUFFER_SIZE + 1);
	if (!buffer)
		return (s);
	free(s);
	buffer_len = ft_strlen(buffer);
	s = malloc(sizeof(char) * buffer_len + 1);
	if (!s)
		return (NULL);
	ft_strlcpy(s, buffer, buffer_len + 1);
	return (s);
}

int	read_and_add_to_string(int fd, ssize_t *ret, char **s, char *buffer)
{
	char	*tmp;

	*ret = read(fd, buffer, BUFFER_SIZE);
	if (*ret == -1)
	{
		free(*s);
		return (0);
	}
	buffer[*ret] = '\0';
	tmp = *s;
	*s = ft_strjoin(tmp, buffer);
	free(tmp);
	if (!s)
		return (0);
	return (1);
}

int	init_s(char **s)
{
	*s = malloc(sizeof(char));
	if (!*s)
		return (0);
	*s[0] = '\0';
	return (1);
}

/*Return  a line read from the file descriptor fd.
Return NULL if there is nothing left to read or in case of error*/
char	*get_next_line(int fd)
{
	char		*s;
	static char	buffer[BUFFER_SIZE + 1];
	ssize_t		ret;

	if (init_s(&s) == 0)
		return (NULL);
	if (contains_eol(buffer))
	{
		s = retrieve_unused_buffer(s, buffer);
		if (!s)
			return (NULL);
	}
	ret = 1;
	while (!contains_eol(s) && ret > 0)
		if (!read_and_add_to_string(fd, &ret, &s, buffer))
			return (NULL);
	if (ret == 0 && ft_strlen(s) == 0)
	{
		free(s);
		return (NULL);
	}
	s = pop_line(s);
	if (!s)
		return (NULL);
	return (s);
}

/*
#include <stdio.h>
#include <fcntl.h>
int main()
{
	char *s;
	int fd = open("test", O_RDONLY);
	s = get_next_line(fd);
	printf("%s", s);
	free(s);
	s = get_next_line(fd);
	printf("%s", s);
	free(s);
	s = get_next_line(fd);
	printf("%s", s);
	free(s);
	s = get_next_line(fd);
	printf("%s", s);
	free(s);
	// s = get_next_line(fd);
	// printf("%s", s);
	// free(s);
	// s = get_next_line(fd);
	// printf("%s", s);
	// free(s);
	// s = get_next_line(fd);
	// printf("%s", s);
	// free(s);
	// s = get_next_line(fd);
	// printf("%s", s);
	// free(s);
	// while (s != NULL)
	// {
	// 	free(s);
	// 	s = get_next_line(fd);
	// 	printf("%s", s);
	// }
	close(fd);

	return 0;
}
*/
