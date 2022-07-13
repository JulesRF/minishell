/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 10:50:48 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/13 17:10:26 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_input_file(t_token **cur, t_token **commands, t_redir *redir)
{
	char	*input_file;

	if (redir->input_redir != -1)
		close(redir->input_redir);
	input_file = (*cur)->next->content;
	ft_delete_token(commands, (*cur)->next);
	ft_delete_token(commands, *cur);
	redir->input_redir = open(input_file, O_RDONLY);
	if (redir->input_redir == -1)
		return (handle_errno(input_file, 1, NULL, NULL));
	return (0);
}

int	set_output_file(t_token **cur, t_token **commands, t_redir *redir,
	int append)
{
	char	*output_file;

	if (redir->output_redir != -1)
		close(redir->output_redir);
	output_file = (*cur)->next->content;
	ft_delete_token(commands, (*cur)->next);
	ft_delete_token(commands, *cur);
	if (append == 1)
		redir->output_redir = open(output_file, O_CREAT | O_WRONLY
				| O_APPEND, S_IRWXU);
	else
		redir->output_redir = open(output_file, O_CREAT | O_WRONLY
				| O_TRUNC, S_IRWXU);
	if (redir->output_redir == -1)
		return (handle_errno(output_file, 1, NULL, NULL));
	return (0);
}

int	check_token_is_in_out_file(t_token **cur, t_token **commands,
	t_redir *redir)
{
	if ((*cur)->type == 5 && ft_strcmp((*cur)->content, "<") == 0)
	{
		if (set_input_file(cur, commands, redir) == 1)
			return (1);
	}
	else if ((*cur)->type == 5 && ft_strcmp((*cur)->content, ">") == 0)
	{
		if (set_output_file(cur, commands, redir, 0) == 1)
			return (1);
	}
	else if ((*cur)->type == 5 && ft_strcmp((*cur)->content, ">>") == 0)
	{
		if (set_output_file(cur, commands, redir, 1) == 1)
			return (1);
	}
	else if ((*cur)->type == 5 && ft_strcmp((*cur)->content, "<<") == 0)
	{
		ft_delete_token(commands, (*cur)->next);
		ft_delete_token(commands, *cur);
		redir->count_heredocs = redir->count_heredocs + 1;
	}
	return (0);
}

int	ambiguous_redirect(t_token **cur, t_token **commands)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((*cur)->next->content, 2);
	ft_putendl_fd(" ambiguous redirect", 2);
	ft_delete_token(commands, (*cur)->next);
	ft_delete_token(commands, *cur);
	return (2);
}

/**
 * @brief Iterate over the command to find and handle input/output files or
 * heredocs.
 * @param commands a simple command
 * @param redir variables related to redirections
 * @return int 1 if error, 2 if ambiguous redirect, 0 otherwise
 */
int	find_in_out_files(t_token **commands, t_redir *redir)
{
	t_token	*cur;

	cur = *commands;
	while (cur)
	{
		if (cur->type == 5 && ft_strcmp(cur->content, "<<")
			&& cur->next && cur->next->type == 6)
			return (ambiguous_redirect(&cur, commands));
		else if (cur->type == 6)
			ft_delete_token(commands, cur);
		else if (check_token_is_in_out_file(&cur, commands, redir) == 1)
			return (1);
		cur = cur->next;
	}
	if ((redir->count_heredocs > 0) && (redir->count_heredocs
			== redir->nb_heredocs))
	{
		if (redir->input_redir != -1)
			close(redir->input_redir);
		redir->input_redir = dup(redir->heredoc_redir);
		if (redir->heredoc_redir != -1)
			close(redir->heredoc_redir);
		redir->heredoc_redir = -1;
	}
	return (0);
}
