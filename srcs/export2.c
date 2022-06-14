/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:28:12 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/14 14:36:14 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sort the environment variables in ascending order
 * 
 * @param env  environment variables
 * @return char** NULL if fail, a sorted copy of the envrionment
 * otherwise
 */
char	**sort_env(char **env)
{
	char	**sorted_env;
	char	*tmp;
	int		i;
	int		j;

	sorted_env = dup_env(env);
	if (!sorted_env)
		return (NULL);
	i = 0;
	while (sorted_env[i])
	{
		j = i + 1;
		while (sorted_env[j])
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				tmp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (sorted_env);
}

/**
 * @brief Print the envrionment variables
 * 
 * @param sorted_env sorted environment variables
 */
void	print_env(char **sorted_env)
{
	int	i;
	int	j;
	int	endquotes;

	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		j = 0;
		endquotes = 0;
		while (sorted_env[i][j])
		{
			ft_putchar_fd(sorted_env[i][j], 1);
			if (sorted_env[i][j] == '=')
			{
				ft_putchar_fd('"', 1);
				endquotes = 1;
			}
			j++;
		}
		if (endquotes == 1)
			ft_putchar_fd('"', 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}

/**
 * @brief Print the environment variables in ascending order
 * 
 * @param env environment variables
 * @return int 1 if fail, 0 otherwise
 */
int	print_sorted_env(char **env)
{
	char	**sorted_env;

	sorted_env = sort_env(env);
	if (!sorted_env)
		return (1);
	print_env(sorted_env);
	free_strs_array(sorted_env);
	return (0);
}

/**
 * @brief Check if the env var name is valid
 * env var name format : [a-zA-Z_]+[a-zA-Z0-9_]*
 * Also calculate the length of said var name
 * @param s arg string
 * @param name_len pointer to name_len variable, to be calculated in the
 * function
 * @return int 1 if valid, 0 if not
 */
int	var_name_is_valid(char *s, int *name_len)
{
	*name_len = 0;
	if (s && (ft_isalpha(s[0]) == 0 && s[0] != '_'))
	{
		ft_putstr_fd("export: ", 2);
		ft_putstr_fd(s, 2);
		ft_putendl_fd(": not a valid identifier", 2);
		return (0);
	}
	*name_len = 1;
	while (s && s[*name_len] && s[*name_len] != '=')
	{
		if (ft_isalnum(s[*name_len]) == 0 && s[*name_len] != '_')
		{
			ft_putstr_fd("export: ", 2);
			ft_putstr_fd(s, 2);
			ft_putendl_fd(": not a valid identifier", 2);
			return (0);
		}
		*name_len = *name_len + 1;
	}
	return (1);
}

/**
 * @brief Get the var name in the arg string
 * 
 * @param s arg string
 * @param name_len length of the variable name
 * @return char* NULL if malloc fails, the malloc'ed var name string otherwise
 */
char	*get_var_name(char *s, int name_len)
{
	char	*var_name;

	var_name = malloc(sizeof(char) * (name_len + 1));
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, s, name_len + 1);
	return (var_name);
}
