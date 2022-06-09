/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 16:49:40 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/30 19:02:36 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size_t	ft_strlen(const char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] != '\0')
// 		i++;
// 	return (i);
// }

// size_t	ft_strlcpy(char *dst, const char *src, size_t size)
// {
// 	size_t	i;

// 	if (size == 0)
// 		return (ft_strlen(src));
// 	i = 0;
// 	while (src[i] != '\0' && i < size - 1)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	dst[i] = '\0';
// 	return (ft_strlen(src));
// }

// char	*ft_strjoin(char *s1, char const *s2)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*ret_str;

// 	if (!s1 || !s2)
// 		return (NULL);
// 	i = ft_strlen(s1) + ft_strlen(s2);
// 	ret_str = malloc(sizeof(char) * (i + 1));
// 	if (!ret_str)
// 		return (NULL);
// 	i = 0;
// 	j = -1;
// 	while (s1[++j])
// 	{
// 		ret_str[i] = s1[j];
// 		i++;
// 	}
// 	j = -1;
// 	while (s2[++j])
// 	{
// 		ret_str[i] = s2[j];
// 		i++;
// 	}
// 	ret_str[i] = '\0';
// 	return (ret_str);
// }

int	contains_eol(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

// char	*ft_strchr(const char *s, int c)
// {
// 	int		i;
// 	char	*ps;

// 	ps = (char *)s;
// 	i = 0;
// 	while (ps[i])
// 	{
// 		if (ps[i] == (char)c)
// 			return (&ps[i]);
// 		i++;
// 	}
// 	if (c == '\0')
// 		return (&ps[i]);
// 	return (NULL);
// }
