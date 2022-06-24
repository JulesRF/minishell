/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 09:29:43 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/24 11:40:55 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc(int pipe_fd[2], char *line)
{
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
}

void	heredoc_warning(t_list **heredoc_eofs, int *nb_eof)
{
	ft_putstr_fd("minishell: warning: here-document delimited\
 by end-of-file (wanted '", 2);
	ft_putstr_fd((*heredoc_eofs)->content, 2);
	ft_putendl_fd("')", 2);
	*nb_eof = *nb_eof + 1;
	(*heredoc_eofs) = (*heredoc_eofs)->next;
}


void	start_heredoc(t_list *heredoc_eofs, int nb_heredocs, int *pipe_fd)
{
	char	*line;
	int		nb_eof;


	signal(SIGINT, SIG_DFL);
	nb_eof = 0;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			heredoc_warning(&heredoc_eofs, &nb_eof);
		if (line && nb_eof == (nb_heredocs - 1)
			&& ft_strcmp(line, heredoc_eofs->content) != 0)
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

/**
 * @brief Put the program in heredoc mode where the user can type multiple 
 * lines of input that will be passed to next command
 * @param heredoc_eofs list of delimiter strings that marks the end of heredoc
 * @param input_redir the input fd from which the next command can read
 * @param nb_heredocs the overall number of heredocs 
 * @return int 0 if no error, n > 0 otherwise
 */
int	multiple_heredoc(t_list *heredoc_eofs, int *input_redir, int nb_heredocs)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		ret;

	if (pipe(pipe_fd) == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	pid = fork();
	if (pid == -1)
		return (handle_errno("fork", 1, NULL, NULL));
	if (pid == 0)
		start_heredoc(heredoc_eofs, nb_heredocs, pipe_fd);
	else
	{
		signal(SIGQUIT, handle_sigquit_heredoc);
		get_child_status(pid, &ret, 1, 0);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
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
