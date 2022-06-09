/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 19:49:51 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:37:08 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Allocates sufficient memory for a copy of the string s, does the copy, 
and returns a pointer to it.  The pointer may subsequently be
used as an argument to the function free*/
char	*ft_strdup(const char *s)
{
	char	*new_str;
	int		i;

	new_str = ft_calloc((ft_strlen(s) + 1), sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
