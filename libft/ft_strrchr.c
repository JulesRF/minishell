/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 21:08:13 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:46:48 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Locates the last occurrence of c (converted to a char) in the string pointed 
to by s. The terminating null character is considered to be part of the string;
 therefore if c is ‘\0’, the functions locate the terminating ‘\0’*/
char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (s[i])
	{
		if (s[i] == (char)c)
			j = i;
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	if (j != -1)
		return ((char *)&s[j]);
	return (NULL);
}
