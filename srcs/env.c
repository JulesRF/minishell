/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:36:55 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/14 14:37:38 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print environment variables
 * 
 * @param env environment variables
 * @return int 0 when done
 */
int	env_builtin(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
			ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}
