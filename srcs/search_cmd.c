/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 16:20:08 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/16 16:43:57 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_not_found(t_token *command)
{
	char	*cmd_name;

	cmd_name = command->content;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putendl_fd(": command not found", 2);
	return (127);
}

/**
 * @brief Check if the command name is a builtin function
 * 
 * @param command Linked list of command (name) and arguments
 * @return int -1 if the search fails, 1 if execution fails, 0 if no error
 */
int	check_builtin(t_token *command, char ***env, t_list **bin, char *cmd_line)
{
	char	*cmd_name;

	cmd_name = command->content;
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (echo(command));
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (cd(command, env));
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (pwd());
	if (ft_strcmp(cmd_name, "export") == 0)
		return (export(command, env));
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (unset(command, env));
	if (ft_strcmp(cmd_name, "env") == 0)
		return (env_builtin(*env));
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (exit_builtin(command, env, bin, cmd_line));
	return (-1);
}

int	find_executable(t_vars *vars, char **paths, char *cmd_name)
{
	char		*path_to_cmd;
	int			i;
	struct stat	sb;
	t_list		*node;

	i = 0;
	while (paths[i])
	{
		path_to_cmd = ft_strjoin(paths[i], cmd_name);
		if ((stat(path_to_cmd, &sb) == 0 && sb.st_mode & S_IXUSR))
		{
			node = ft_lstnew(path_to_cmd);
			if (!node)
				return (-1);
			ft_lstadd_back(vars->bin, node);
			(*(vars->cmd))->content = path_to_cmd;
			return (exec_cmd((*(vars->cmd)), *(vars->env), vars->pid));
		}
		i++;
		free(path_to_cmd);
	}
	return (-1);
}

/**
 * @brief Check if an executable whose name is that of command exists in
 * one the PATH directories. If an executable is found, it is executed
 * @param command Linked list of command (name) and arguments
 * @param env Environment variables
 * @return int -1 if the search or execution fails
 */
int	check_path(t_vars *vars)
{
	char		*cmd_name;
	char		*path;
	char		**paths;
	int			ret;

	cmd_name = ft_strjoin("/", (*(vars->cmd))->content);
	if (!cmd_name)
		return (-1);
	path = get_env_value("PATH", *(vars->env));
	if (!path)
	{
		free(cmd_name);
		return (-1);
	}
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
	{
		free(cmd_name);
		return (-1);
	}
	ret = find_executable(vars, paths, cmd_name);
	free(cmd_name);
	free_strs(paths);
	return (ret);
}

/**
 * @brief Search the command executable and try execute it
 * 
 * @param command Linked list of command (path or name) and arguments
 * @param env Environment variables
 * @return int -1 if the search fails, 1 if execution fails, 0 if no error
 */
int	search_cmd(t_vars *vars)
{
	char	*cmd_name;
	int		ret;

	ret = 0;
	if (*(vars->cmd) == NULL)
		return (-1);
	cmd_name = (*(vars->cmd))->content;
	if (ft_strchr(cmd_name, '/') == NULL)
	{
		if (ft_strlen(cmd_name) == 0)
			return (cmd_not_found(*(vars->cmd)));
		ret = check_builtin(*(vars->cmd), vars->env, vars->bin, vars->cmd_line);
		if (ret != -1)
			return (ret);
		ret = check_path(vars);
		if (ret != -1)
			return (ret);
		return (cmd_not_found(*(vars->cmd)));
	}
	else
	{
		ret = exec_cmd(*(vars->cmd), *(vars->env), vars->pid);
	}
	return (ret);
}
