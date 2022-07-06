/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 11:24:42 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/06 15:46:38 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Modify the value of a given environment variable
 * 
 * @param s arg string
 * @param var_name the key of the environment variable
 * @param name_len length of the variable name
 * @param env environment variables
 * @return int 1 in case of failure, 0 otherwise
 */
int	set_var_in_env(char *s, char *var_name, int name_len, char ***env)
{
	int		i;
	int		j;
	char	**envv;

	envv = *env;
	i = 0;
	while (envv && envv[i])
	{
		j = 0;
		while (envv[i][j] && var_name[j] && envv[i][j] == var_name[j])
			j++;
		if (j == name_len && (envv[i][j] == '=' || envv[i][j] == '\0') && s[j] != '\0') 
		{
			free(envv[i]);
			envv[i] = ft_strdup(s);
			if (!envv[i])
				return (1);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Copy tmp values into new_env
 * 
 * @param i 
 * @param tmp src
 * @param new_env dest
 * @return int 1 if error, 0 otherwise
 */
int	copy_env(int *i, char ***tmp, char ***new_env)
{
	int	j;

	*i = 0;
	while ((*tmp)[*i])
	{
		(*new_env)[*i] = ft_strdup((*tmp)[*i]);
		if (!(*new_env)[*i])
		{
			j = 0;
			while (j < *i)
			{
				free((*new_env)[j]);
				j++;
			}
			free(*new_env);
			return (1);
		}
		*i = *i + 1;
	}
	return (0);
}

/**
 * @brief Add a new variable to the environment
 * 
 * @param s arg string
 * @param env environment variables
 * @return int 1 in case of failure, 0 otherwise
 */
int	add_var_to_env(char *s, char ***env)
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
	if (copy_env(&i, &tmp, &new_env) == 1)
		return (1);
	new_env[i] = ft_strdup(s);
	if (!new_env[i])
	{
		free_strs_array(new_env);
		return (1);
	}
	i++;
	new_env[i] = NULL;
	*env = new_env;
	free_strs_array(tmp);
	return (0);
}

/**
 * @brief If the variable already exists, update its value, if not,
 * create the new variable
 * @param command Linked list of command (name) and arguments
 * @param env environment variables
 * @param name_len 
 * @return int 1 in case of failure, 0 otherwise
 */
int	add_or_set_to_env(t_token *command, char ***env, int name_len)
{
	char	*var_name;
	char	*value;
	int		ret;

	ret = 0;
	if (command->content[name_len - 1] == '+')
		ret = concat_to_env(command, env, name_len);
	else
	{
		var_name = get_var_name(command->content, name_len);
		if (!var_name)
			return (1);
		value = get_env_value(var_name, *env);
		if (value == NULL)
			ret = add_var_to_env(command->content, env);
		else 
			ret = set_var_in_env(command->content, var_name, name_len, env);
		free(var_name);
		free(value);
	}
	return (ret);
}

/**
 * @brief Add a variable to the environment or change its value
 * if a variable with the same key already exists
 * @param command Linked list of command (name) and arguments
 * @param env environment variables
 * @return int 1 in case of failure, 0 otherwise
 */
int	export(t_token *command, char ***env)
{
	int		ret;
	int		name_len;

	if (command != NULL)
		command = command->next;
	if (command == NULL)
		return (print_sorted_env(*env));
	ret = 0;
	while (command)
	{
		if (command->type == 2)
		{
			if (var_name_is_valid(command->content, &name_len) == 1)
				ret = add_or_set_to_env(command, env, name_len);
			else
				ret = 1;
		}
		command = command->next;
	}
	return (ret);
}
