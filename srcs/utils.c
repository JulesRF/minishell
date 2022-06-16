/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 10:15:45 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/16 13:28:32 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Return a string representation of the command and its arguments
 * 
 * @param command Linked list of command (name) and arguments
 * @return char* NULL if error, the string containing the command and its 
 * arguments otherwise
 */
char	*cmd_to_str(t_token *command)
{
	char	*args_str;
	char	*tmp;

	args_str = malloc(1);
	if (!args_str)
		return (NULL);
	args_str[0] = '\0';
	while (command && (command->type == 2 || command->type == 4))
	{
		tmp = args_str;
		args_str = ft_strjoin(args_str, command->content);
		free(tmp);
		if (!args_str)
			return (NULL);
		command = command->next;
	}
	return (args_str);
}

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
