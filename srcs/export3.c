/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 08:41:27 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/19 09:41:18 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_char_from_str(int index, char **str)
{
	int		i;
	char	*s;

	i = index;
	s = *str;
	while ((s + 1) && s[i + 1])
	{
		s[i] = s[i + 1];
		i++;
	}
	s[i] = '\0';
}

char	*get_var_name_and_value(t_token *command, int *name_len,
	char **var_name, char ***env)
{
	char	*value;

	*name_len = *name_len - 1;
	*var_name = get_var_name(command->content, (*name_len));
	if (!*var_name)
	{
		*name_len = -1;
		return (NULL);
	}
	value = get_env_value(*var_name, *env);
	rm_char_from_str((*name_len), &(command->content));
	return (value);
}

char	*concat_env_vars(char *var_name, char *value, t_token *command,
	int name_len)
{
	char	*concat;
	char	*tmp;

	concat = ft_strjoin(var_name, "=");
	tmp = concat;
	concat = ft_strjoin(concat, value);
	if (tmp)
		free(tmp);
	tmp = concat;
	concat = ft_strjoin(concat, &(command->content[name_len + 1]));
	if (tmp)
		free(tmp);
	return (concat);
}

int	concat_to_env(t_token *command, char ***env, int name_len)
{
	char	*var_name;
	char	*value;
	char	*concat;
	int		ret;

	ret = 0;
	value = get_var_name_and_value(command, &name_len, &var_name, env);
	if (name_len == -1)
		return (1);
	if (value == NULL)
		ret = add_var_to_env(command->content, env);
	else
	{
		concat = concat_env_vars(var_name, value, command, name_len);
		if (!concat)
			ret = 1;
		else
		{
			ret = set_var_in_env(concat, var_name, name_len, env);
			free(concat);
		}
	}
	free(var_name);
	free(value);
	return (ret);
}
