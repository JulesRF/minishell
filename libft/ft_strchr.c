/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 20:43:52 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:36:21 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Locates the first occurrence of c (converted to a char) in the string 
pointed to by s. The terminating null character is considered
to be part of the string; therefore if c is ‘\0’, the functions locate 
the terminating ‘\0’*/
char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*ps;

	ps = (char *)s;
	i = 0;
	while (ps[i])
	{
		if (ps[i] == (char)c)
			return (&ps[i]);
		i++;
	}
	if (c == '\0')
		return (&ps[i]);
	return (NULL);
}
