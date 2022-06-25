/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 15:35:33 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/25 15:58:52 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_oldpwd(char ***env)
{
	char	*old_str;

	old_str = get_env_var("OLDPWD", *env);
	if (old_str)
	{
		rm_var_from_env(old_str, env);
		free(old_str);
	}
	return (0);
}

int	get_current_pwd(char **new_str)
{
	char	buffer[BUFFER_SIZE];

	if (getcwd(buffer, BUFFER_SIZE) == NULL)
		handle_errno("chdir: error retrieving current directory: \
getcwd: cannot access parent directories", 1, NULL, NULL);
	*new_str = ft_strjoin("PWD=", buffer);
	if (!*new_str)
		return (handle_error("Memory allocation error", 1, NULL, NULL));
	return (0);
}

int	path_exists(char *path)
{
	char	buffer[BUFFER_SIZE];

	if (getcwd(buffer, BUFFER_SIZE) == NULL && path[0] == '.')
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (0);
	}
	return (1);
}

int	set_new_dir(char **path, char **old_pwd, char **env)
{
	*old_pwd = get_env_value("PWD", env);
	if (chdir(*path) == -1)
	{
		*path = ft_strjoin("cd: ", *path);
		handle_errno(*path, 1, NULL, NULL);
		return (1);
	}
	return (0);
}
