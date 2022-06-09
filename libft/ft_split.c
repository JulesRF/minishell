/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 15:01:49 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:34:34 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}

static int	ft_word_count(char const *s, int c)
{
	int	i;
	int	words;

	if (ft_strlen(s) == 0)
		return (0);
	if (c == 0)
		return (1);
	i = 0;
	words = 0;
	while (s[i] != '\0')
	{
		while (s[i] != c && s[i] != '\0')
		{
			if (s[i + 1] == c && s[i] != c)
				words++;
			i++;
		}
		while (s[i] != '\0' && s[i] == c)
			i++;
	}
	if (s[i - 1] != c)
		words++;
	return (words);
}

static int	ft_cpy_in_tab(char *tab, char const *s, char c, int i)
{
	int	x;

	x = 0;
	while (s[i] != c && s[i])
	{
		tab[x] = s[i];
		i++;
		x++;
	}
	tab[x] = '\0';
	return (i);
}

static void	*free_tab(char **tab, int j)
{
	int	i;

	i = 0;
	while (i < j)
		free(tab[i]);
	free(tab);
	return (NULL);
}

/*Allocates (with malloc) and returns an array
of strings obtained by splitting ’s’ using the
character ’c’ as a delimiter.  The array must end
with a NULL pointer*/
char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**tab;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	tab = malloc(sizeof(tab) * (ft_word_count(s, c) + 1));
	if (!tab)
		return (NULL);
	while (j < ft_word_count(s, c))
	{
		while (s[i] == c && s[i])
			i++;
		tab[j] = malloc(sizeof(char) * (ft_word_len(s + i, c) + 1));
		if (!tab[j])
			return (free_tab(tab, j));
		i = ft_cpy_in_tab(tab[j], s, c, i);
		j++;
	}
	tab[j] = 0;
	return (tab);
}
