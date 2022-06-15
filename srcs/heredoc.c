/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 09:29:43 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/15 15:47:24 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc(int pipe_fd[2], char *line)
{
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
}

/**
 * @brief Put the program in heredoc mode where the user can type multiple 
 * lines of input that will be passed to next command
 * @param heredoc_eof delimiter string that marks the end of heredoc
 * @return int -1 if error, the input fd from which the next command can
 * read otherwise
 */
int	heredoc(char *heredoc_eof)
{
	char	*line;
	int		pipe_fd[2];
	int		input_redir;

	if (pipe(pipe_fd) == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	while (1)
	{
		line = readline("> ");
		if (line && (ft_strcmp(line, heredoc_eof) == 0))
			break ;
		write_heredoc(pipe_fd, line);
		free (line);
	}
	free(line);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	input_redir = dup(0);
	if (input_redir == -1)
		return (handle_errno("dup", -1, NULL, NULL));
	return (input_redir);
}
