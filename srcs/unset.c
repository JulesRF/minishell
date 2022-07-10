/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:38:50 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/10 09:47:12 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Fill the new environment, excluding the variable to remove
 * 
 * @param new_env new environment
 * @param env current environment
 * @param s arg string
 * @param key_len 
 * @return int 1 if fail, 0 otherwise 
 */
int	fill_new_environment(char ***new_env, char **env, char *s, int key_len)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (env[i])
	{
		k = 0;
		while (env[i][k] && s[k] && env[i][k] == s[k])
			k++;
		if (k != key_len || (env[i][k] != '=' && env[i][k] != '\0'))
		{
			(*new_env)[j] = ft_strdup(env[i]);
			if (!(*new_env)[j])
			{
				free_strs_array(*new_env);
				return (1);
			}
			j++;
		}
		i++;
	}
	(*new_env)[j] = NULL;
	return (0);
}

/**
 * @brief Remove the variable from the environment
 * 
 * @param s arg string
 * @param env environment variables
 * @return int 1 if fail, 0 otherwise
 */
int	rm_var_from_env(char *s, char ***env)
{
	int		i;
	char	**new_env;
	char	**tmp;

	i = 0;
	tmp = *env;
	while (tmp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	fill_new_environment(&new_env, *env, s, ft_strlen(s));
	*env = new_env;
	free_strs_array(tmp);
	return (0);
}

/**
 * @brief Check if the argument is a valid parameter for unset
 * env var name format : [a-zA-Z_]+[a-zA-Z0-9_]*
 * Also calculate the length of said var name
 * @param s arg string
 * @return int 1 if valid, 0 if not
 */
int	parameter_is_valid(char *s)
{
	int	i;

	if (s && (ft_isalpha(s[0]) == 0 && s[0] != '_'))
	{
		ft_putstr_fd("unset: ", 2);
		ft_putstr_fd(s, 2);
		ft_putendl_fd(": invalid parameter name", 2);
		return (0);
	}
	i = 1;
	while (s && s[i])
	{
		if (ft_isalnum(s[i]) == 0 && s[i] != '_')
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(s, 2);
			ft_putendl_fd(": invalid parameter name", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	find_and_rm_var(t_token *command, char ***env)
{
	char	*value;

	value = get_env_value(command->content, *env);
	if (value != NULL)
	{
		if (rm_var_from_env(command->content, env) == 1)
		{
			free(value);
			return (1);
		}
	}
	free(value);
	return (0);
}

/**
 * @brief Remove a variable from the environment
 * 
 * @param command Linked list of command (name) and arguments
 * @param env environment variables
 * @return int 1 if fail, 0 otherwise
 */
int	unset(t_token *command, char ***env)
{
	int		ret;

	ret = 0;
	if (command != NULL)
		command = command->next;
	if (command == NULL)
		return (0);
	while (command)
	{
		if (command->type == 2)
		{
			if (parameter_is_valid(command->content) == 1)
			{
				if (find_and_rm_var(command, env) == 1)
					return (1);
			}
			else
				ret = 1;
		}
		command = command->next;
	}
	return (ret);
}
