/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 12:23:13 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:47:30 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isinset(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static size_t	ft_findbegin(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i])
	{
		if (! ft_isinset(s1[i], set))
			return (i);
		i++;
	}
	return (0);
}

static size_t	ft_findend(char const *s1, char const *set)
{
	size_t	i;
	size_t	endofstr;

	endofstr = ft_strlen(s1) - 1;
	i = endofstr;
	while (i > 0)
	{
		if (! ft_isinset(s1[i], set))
			return (i);
		i--;
	}
	return (0);
}

static char	*empty_malloc(void)
{
	char	*ret_str;

	ret_str = malloc(sizeof(char));
	if (!ret_str)
		return (NULL);
	ret_str[0] = '\0';
	return (ret_str);
}

/*Allocates (with malloc) and returns a copy of
’s1’ with the characters specified in ’set’ removed
from the beginning and the end of the string*/
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*ret_str;

	if (!s1 || !set)
		return (NULL);
	i = ft_findbegin(s1, set);
	j = ft_findend(s1, set);
	if ((i == 0 && j == 0) || ft_strlen(s1) == 0)
		return (empty_malloc());
	ret_str = malloc(sizeof(char) * (j - i + 2));
	if (!ret_str)
		return (NULL);
	k = 0;
	while (i <= j)
	{
		ret_str[k] = s1[i];
		k++;
		i++;
	}
	ret_str[k] = '\0';
	return (ret_str);
}
