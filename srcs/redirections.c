/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 10:50:48 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/15 20:32:45 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_nb_cmd(t_token *commands)
{
	int	count;

	count = 1;
	while (commands)
	{
		if (commands->type == 1 && (ft_strcmp(commands->content, "|") == 0))
			count++;
		commands = commands->next;
	}
	return (count);
}


int	handle_err_free_lst(char *error_msg, int ret, t_list **to_free)
{
	if (to_free != NULL)
		ft_garbage(to_free);
	if (error_msg)
		ft_putendl_fd(error_msg, 2);
	return (ret);
}

int	handle_errno_free_lst(char *error_msg, int ret, t_list **to_free)
{
	perror(error_msg);
	if (to_free != NULL)
		ft_garbage(to_free);
	return (ret);
}

int find_input_and_output_files(t_token **commands, t_redir *redir, t_list **bin)
{
	t_token *cur;
	t_list *heredoc_eofs;
	char *heredoc_eof;
	t_list *node;
	char *input_file;
	char *output_file;
	int nb_heredocs;
	int ret;
	
	heredoc_eofs = NULL;
	nb_heredocs = 0;
	redir->input_redir = -1;
	redir->output_redir = -1;
	cur = *commands;
	while (cur)
	{
		if (cur->type == 5 && ft_strcmp(cur->content, "<") == 0)
		{
			if (redir->input_redir != -1)
				close(redir->input_redir);
			input_file = cur->next->content; //check if next NULL ? devrait pas l'etre apres parsing
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			redir->input_redir = open(input_file, O_RDONLY);
			if (redir->input_redir == -1)
				return handle_errno_free_lst(input_file, -1, &heredoc_eofs);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, ">") == 0)
		{
			if (redir->output_redir != -1)
				close(redir->output_redir);
			output_file = cur->next->content; //check if next NULL ? devrait pas l'etre apres parsing
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			redir->output_redir = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			if (redir->output_redir == -1)
					return handle_errno_free_lst(output_file, -1, &heredoc_eofs);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, ">>") == 0)
		{
			if (redir->output_redir != -1)
				close(redir->output_redir);
			output_file = cur->next->content; //check if next NULL ? devrait pas l'etre apres parsing
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			redir->output_redir = open(output_file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
			if (redir->output_redir == -1)
					return handle_errno_free_lst(output_file, -1, &heredoc_eofs);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, "<<") == 0)
		{
			heredoc_eof = ft_strdup(cur->next->content);
			if (!heredoc_eof)
				return handle_err_free_lst(NULL, -1, &heredoc_eofs);
			node = ft_lstnew(heredoc_eof);
			if (!node)
				return handle_err_free_lst(NULL, -1, &heredoc_eofs);
			ft_lstadd_back(&heredoc_eofs, node);
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			nb_heredocs++;
		}
		cur = cur->next;
	}
	if (nb_heredocs > 0)
	{
		if (redir->input_redir != -1)
			close(redir->input_redir);
		ft_lstadd_back(bin, heredoc_eofs);
		ret = multiple_heredoc(heredoc_eofs, &(redir->input_redir), nb_heredocs);
		if (ret != 0)
			return ret;
	}
	return 0;
}
