/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 10:03:59 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/14 10:50:33 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print the name of the current/working directory
 * 
 * @return int 1 in case of failure, 0 otherwise
 */
int	pwd(void)
{
	char	buffer[BUFFER_SIZE];
	char	*ret;

	ret = getcwd(buffer, BUFFER_SIZE);
	if (ret == NULL)
		return (handle_error("getcwd", 1, NULL, NULL));
	ft_putendl_fd(ret, 1);
	return (0);
}
