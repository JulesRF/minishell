/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 09:29:43 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/11 10:10:17 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_heredoc_eof_to_list(t_token **cur, t_token **commands, t_redir *redir)
{
	char	*heredoc_eof;
	t_list	*node;

	(void)commands;
	if ((*cur)->next == NULL || (*cur)->next->type != 2)
	{
		redir->missing_heredoc_delim = 1;
		return (0);
	}
	if ((*cur)->next)
		heredoc_eof = ft_strdup((*cur)->next->content);
	if (!heredoc_eof)
		return (1);
	node = ft_lstnew(heredoc_eof);
	if (!node)
	{
		free(heredoc_eof);
		return (1);
	}
	if ((*cur)->next)
		node->qt = (*cur)->next->qt;
	ft_lstadd_back(&(redir->heredoc_eofs), node);
	redir->count_heredocs = redir->count_heredocs + 1;
	return (0);
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

void	start_heredoc(t_list	*heredoc_eof, t_redir *redir, int *pipe_fd,
	t_data *vars)
{
	char	*line;
	int		nb_eof;

	signal(SIGINT, SIG_DFL);
	nb_eof = 0;
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			heredoc_warning(&heredoc_eof, &nb_eof);
		if (line && nb_eof == (redir->count_heredocs - 1)
			&& ft_strcmp(line, heredoc_eof->content) != 0)
			write_heredoc(pipe_fd, line, vars, heredoc_eof->qt);
		if (line && (ft_strcmp(line, heredoc_eof->content) == 0))
		{
			nb_eof++;
			heredoc_eof = heredoc_eof->next;
		}
		if (nb_eof == redir->count_heredocs)
			break ;
		free (line);
	}
	free(line);
	clean_and_close_heredoc(pipe_fd, vars, redir);
}

/**
 * @brief Put the program in heredoc mode where the user can type multiple 
 * lines of input that will be passed to next command
 * @param heredoc_eofs list of delimiter strings that marks the end of heredoc
 * @param input_redir the input fd from which the next command can read
 * @param nb_heredocs the overall number of heredocs 
 * @return int 0 if no error, n > 0 otherwise
 */
int	multiple_heredoc(t_redir *redir, t_data *vars)
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
		start_heredoc(redir->heredoc_eofs, redir, pipe_fd, vars);
	else
	{
		signal(SIGQUIT, handle_sigquit_heredoc);
		get_child_status(pid, &ret, 1, 0);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
		if (ret != 0 || redir->missing_heredoc_delim == 1)
			return (close_pipe_and_ret(pipe_fd, ret));
	}
	close(pipe_fd[1]);
	redir->heredoc_redir = pipe_fd[0];
	return (0);
}

/**
 * @brief Find and handle all heredocs in command
 * 
 * @param commands entire command
 * @param redir variables related to redirections
 * @return int 
 */
int	find_heredocs(t_token **commands, t_redir *redir, t_data *vars)
{
	t_token	*cur;
	int		ret;

	redir->heredoc_redir = -1;
	redir->heredoc_eofs = NULL;
	redir->count_heredocs = 0;
	redir->missing_heredoc_delim = 0;
	ret = 0;
	cur = *commands;
	while (cur && redir->missing_heredoc_delim == 0)
	{
		if (cur->type == 5 && ft_strcmp(cur->content, "<<") == 0)
			if (add_heredoc_eof_to_list(&cur, commands, redir) == 1)
				return (1);
		cur = cur->next;
	}
	if (redir->count_heredocs > 0)
	{
		ret = multiple_heredoc(redir, vars);
		ft_garbage(&(redir->heredoc_eofs));
	}
	if (ret != 0)
		return (ret);
	return (0);
}
