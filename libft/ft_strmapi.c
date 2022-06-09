/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 17:48:59 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:43:07 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Applies the function ’f’ to each character of the
string ’s’, and passing its index as first argument
to create a new string (with malloc) resulting
from successive applications of ’f’*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		len;
	int		i;
	char	*ret_str;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	ret_str = malloc(sizeof(char) * (len + 1));
	if (!ret_str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		ret_str[i] = f(i, s[i]);
		i++;
	}
	ret_str[i] = '\0';
	return (ret_str);
}
