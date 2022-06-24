/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 11:17:41 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/24 13:28:40 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_error_status(char *path)
{
	DIR	*folder;
	int	f;
	int	ret;

	f = open(path, O_WRONLY);
	folder = opendir(path);
	if (f == -1 && folder == NULL)
		ret = 127;
	else
		ret = 126;
	if (folder)
		closedir(folder);
	close(f);
	return (ret);
}

int	get_child_status(int pid, int *ret, int change_sig, int ignore_err)
{
	int		status;

	if (change_sig)
	{
		signal(SIGINT, handle_sigint_no_prompt);
		signal(SIGQUIT, handle_sigquit_heredoc);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		if (ignore_err)
			return (-1);
		return (handle_errno("waitpid", 1, NULL, NULL));
	}
	if (WIFEXITED(status))
		*ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		*ret = WTERMSIG(status);
		*ret = *ret + 128;
	}
	if (change_sig)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGINT, handle_sigquit);
	}
	if (*ret == 130)
		write(1, "\n", 1);
	if (*ret == 131 && !ignore_err)
		write(1, "Quit (core dumped)\n", 19);
	return (0);
}

void	start_exec(char	**args, char **env)
{
	int	ret;

	execve(args[0], args, env);
	handle_errno(args[0], 1, NULL, NULL);
	ret = find_error_status(args[0]);
	exit(ret);
}

/**
 * @brief Execute the executable file named after command in a child process.
 * The parent process waits for the child to die
 * 
 * @param command Linked list of command (path) and arguments
 * @param env Environment variables
 * @param pid 0 : this process is child of minishell, -1 this process
 * is minishell
 * @return int 1 if execution fails, 0 if no error
 */
int	exec_cmd(t_token *command, char **env, pid_t pid)
{
	int		ret;
	char	**args;

	ret = 0;
	args = cmd_to_strs(command);
	if (!args)
		return (handle_error("cmd_to_str failed", 1, NULL, NULL));
	if (pid == -1)
	{
		pid = fork();
		if (pid == -1)
		{
			free(args);
			return (handle_errno("fork", 1, NULL, NULL));
		}
		else if (pid == 0)
			start_exec(args, env);
		else
			if (get_child_status(pid, &ret, 1, 0) == 1)
				return (1);
	}
	else
		start_exec(args, env);
	free(args);
	return (ret);
}

/**
 * @brief Fork the process to execute the command
 * 
 * @param vars variables related to command execution
 * @param redir variables related to redirections
 * @return int 1 if error, should not return otherwise
 */
int	fork_exec(t_vars *vars, t_redir *redir)
{
	vars->pid = fork();
	if (vars->pid == -1)
		return (handle_errno("fork failed", 1, redir->cmd_table, NULL));
	if (vars->pid == 0)
	{
		close(redir->fdin);
		redir->ret = search_cmd(vars);
		exit(redir->ret);
	}
	return (0);
}
