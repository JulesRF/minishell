/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 11:17:41 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/15 12:21:07 by vfiszbin         ###   ########.fr       */
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

int	get_child_status(int pid, int *ret)
{
	int		status;

	signal(SIGINT, handle_sigint_no_prompt);
	if (waitpid(pid, &status, 0) == -1)
		return (handle_errno("waitpid", 1, NULL, NULL));
	if (WIFEXITED(status))
		*ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		*ret = WTERMSIG(status);
		*ret = *ret + 128;
	}
	signal(SIGINT, handle_sigint);
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
			return (handle_errno("fork", 1, NULL, args));
		else if (pid == 0)
			start_exec(args, env);
		else
			if (get_child_status(pid, &ret) == 1)
				return (1);
	}
	else
		start_exec(args, env);
	free(args);
	return (ret);
}
