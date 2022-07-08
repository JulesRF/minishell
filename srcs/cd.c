/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 10:06:53 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/08 11:34:11 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Set path to OLDPWD if it exists
 * 
 * @param path new path of the working directory
 * @param env environment variables
 * @return int 1 if OLDPWD is not set, 0 otherwise
 */
int	cd_oldpwd(char **path, char **env, int *path_allocated)
{
	char	*oldpwd;

	oldpwd = get_env_value("OLDPWD", env);
	if (!oldpwd)
		return (handle_error("cd: OLDPWD not set", 1, NULL, NULL));
	*path = oldpwd;
	printf("%s\n", *path);
	*path_allocated = 1;
	return (0);
}

/**
 * @brief Update PWD or OLDPWD by setting it to the current working directory
 * 
 * @param pwd PWD or OLDPWD
 * @param strlen 3 or 6
 * @param env environment variables
 * @return int 1 if error, 0 otherwise
 */
int	update_pwd(char *pwd, int strlen, char ***env, char *old_pwd)
{
	char	*new_str;
	char	*old_str;

	if (old_pwd == NULL && strcmp(pwd, "OLDPWD") == 0)
		return (unset_oldpwd(env));
	if (strcmp(pwd, "PWD") == 0)
	{
		if (get_current_pwd(&new_str) == 1)
			return (1);
	}
	else
		new_str = ft_strjoin("OLDPWD=", old_pwd);
	old_str = get_env_value(pwd, *env);
	if (!old_str)
	{
		if (add_var_to_env(new_str, env) == 1)
			return (handle_error("Memory allocation error", 1, new_str, NULL));
	}
	else
		if (set_var_in_env(new_str, pwd, strlen, env) == 1)
			return (handle_error("Memory allocation error", 1, new_str,
					old_str));
	free(new_str);
	free(old_str);
	return (0);
}

int	free_path(char *path, int path_allocated, int ret, char *old_pwd)
{
	if (path_allocated && path)
		free(path);
	if (old_pwd)
		free(old_pwd);
	return (ret);
}

/**
 * @brief Set the new path for the working directory to HOME if no arguments
 * are provided
 * @param command Linked list of command (name) and arguments
 * @param env environment variables
 * @param path new path for working directory
 * @param path_allocated boolean
 * @return int 1 if error, 0 otherwise
 */
int	check_no_arg(t_token **command, char **env, char **path,
	int *path_allocated)
{
	*path_allocated = 0;
	if (*command != NULL)
		*command = (*command)->next;
	if (*command == NULL)
	{
		*path = get_env_value("HOME", env);
		if (!*path)
			return (handle_error("cd: HOME not set", 1, NULL, NULL));
		*path_allocated = 1;
	}
	return (0);
}

/**
 * @brief Change the working directory to the given path
 * 
 * @param command Linked list of command (name) and arguments, 
 * which should include the path
 * @param env environment variables
 * @return int 0 on success, 1 on error
 */
int	cd(t_token *command, char ***env)
{
	char	*path;
	char	*old_pwd;
	int		path_allocated;

	if (check_no_arg(&command, *env, &path, &path_allocated) == 1)
		return (1);
	if (command == NULL)
		(void)0;
	else if (command != NULL && command->next != NULL)
		return (handle_error("cd: too many arguments", 1, NULL, NULL));
	else if (ft_strcmp(command->content, "-") == 0)
	{
		if (cd_oldpwd(&path, *env, &path_allocated) == 1)
			return (1);
	}
	else
		path = command->content;
	if (path_exists(path) == 0)
		return (free_path(path, path_allocated, 1, NULL));
	if (set_new_dir(&path, &old_pwd, *env, path_allocated) == 1)
		return (free_path(path, 1, 1, old_pwd));
	if (update_pwd("PWD", 3, env, NULL) == 1
		|| update_pwd("OLDPWD", 6, env, old_pwd) == 1)
		return (free_path(path, path_allocated, 1, old_pwd));
	return (free_path(path, path_allocated, 0, old_pwd));
}
