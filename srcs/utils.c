/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 10:15:45 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/19 08:49:58 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_error_str(char *error_msg)
{
	ft_putendl_fd(error_msg, 2);
	return (NULL);
}

/**
 * @brief Get the env value for key
 * 
 * @param key key to search
 * @param env environment variables
 * @return char* NULL if not found, the allocated value otherwise
 */
char	*get_env_value(char *key, char **env)
{
	int		i;
	int		j;
	int		key_len;
	char	*value;

	key_len = ft_strlen(key);
	i = -1;
	while (env && env[++i])
	{
		j = 0;
		while (env[i][j] && key[j] && env[i][j] == key[j])
			j++;
		if (j == key_len && (env[i][j] == '=' || env[i][j] == '\0'))
		{
			if (env[i][j] == '=')
				value = ft_strdup(&env[i][j + 1]);
			else
				value = ft_strdup(&env[i][j]);
			if (!value)
				return (handle_error_str("Memory allocation error"));
			return (value);
		}
	}
	return (NULL);
}

/**
 * @brief Get the env variable who matches the given key
 * 
 * @param key key to search
 * @param env environment variables
 * @return char* NULL if not found, the allocated value otherwise
 */
char	*get_env_var(char *key, char **env)
{
	int		i;
	int		j;
	int		key_len;
	char	*value;

	key_len = ft_strlen(key);
	i = -1;
	while (env && env[++i])
	{
		j = 0;
		while (env[i][j] && key[j] && env[i][j] == key[j])
			j++;
		if (j == key_len && (env[i][j] == '=' || env[i][j] == '\0'))
		{
				value = ft_strdup(env[i]);
			if (!value)
				return (handle_error_str("Memory allocation error"));
			return (value);
		}
	}
	return (NULL);
}

int	handle_error(char *error_msg, int ret_value, char *to_free1, char *to_free2)
{
	if (to_free1 != NULL)
		free(to_free1);
	if (to_free2 != NULL)
		free(to_free2);
	ft_putendl_fd(error_msg, 2);
	return (ret_value);
}

int	handle_errno(char *error_msg, int ret, t_token **to_free1,
	t_list **to_free2)
{
	perror(error_msg);
	if (to_free1 != NULL)
		free(to_free1);
	if (to_free2 != NULL)
		ft_garbage(to_free2);
	return (ret);
}
