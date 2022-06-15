/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 09:29:43 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/15 20:11:20 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc(int pipe_fd[2], char *line)
{
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
}


int	multiple_heredoc(t_list *heredoc_eofs, int *input_redir, int nb_heredocs)
{
	char	*line;
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	int ret;
	int nb_eof;

	if (pipe(pipe_fd) == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	pid = fork();
	if (pid == -1)
		return (handle_errno("fork", 1, NULL, NULL));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		nb_eof = 0;
		while (1)
		{
			line = readline("> ");
			if (nb_eof == (nb_heredocs - 1) && ft_strcmp(line, heredoc_eofs->content) != 0)
				write_heredoc(pipe_fd, line);
			if (line && (ft_strcmp(line, heredoc_eofs->content) == 0))
			{
				nb_eof++;
				heredoc_eofs = heredoc_eofs->next;
			}
			if (nb_eof == nb_heredocs)
				break ;
			free (line);
		}
		free(line);
		exit(0);
	}
	else
	{
		ret = 0;
		signal(SIGINT, handle_sigint_no_prompt);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			ret = WTERMSIG(status);
			ret += 128;
		}
			signal(SIGINT, handle_sigint);
		if (ret != 0)
			return (ret);
	}
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	*input_redir = dup(0);
	if (*input_redir == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	return (0);
}

/**
 * @brief Put the program in heredoc mode where the user can type multiple 
 * lines of input that will be passed to next command
 * @param heredoc_eof delimiter string that marks the end of heredoc
 * @return int -1 if error, the input fd from which the next command can
 * read otherwise
 */
int	heredoc(char *heredoc_eof, int *input_redir)
{
	char	*line;
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	int ret;

	if (pipe(pipe_fd) == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	pid = fork();
	if (pid == -1)
		return (handle_errno("fork", 1, NULL, NULL));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		while (1)
		{
			line = readline("> ");
			if (line && (ft_strcmp(line, heredoc_eof) == 0))
				break ;
			write_heredoc(pipe_fd, line);
			free (line);
		}
		free(line);
		exit(0);
	}
	else
	{
		ret = 0;
		signal(SIGINT, handle_sigint_no_prompt);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			ret = WTERMSIG(status);
			ret += 128;
		}
			signal(SIGINT, handle_sigint);
		if (ret != 0)
			return (ret);
	}
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	*input_redir = dup(0);
	if (*input_redir == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	return (0);
}
