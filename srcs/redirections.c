/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 10:50:48 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/16 12:16:18 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_nb_cmd_and_heredocs(t_token *commands, int *nb_heredocs)
{
	int	count;

	*nb_heredocs = 0;
	count = 1;
	while (commands)
	{
		if (commands->type == 1 && (ft_strcmp(commands->content, "|") == 0))
			count++;
		else if (commands->type == 5 && ft_strcmp(commands->content, "<<") == 0)
			*nb_heredocs = *nb_heredocs + 1;
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

int find_input_and_output_files(t_token **commands, t_redir *redir, t_list **bin, t_list **heredoc_eofs, int nb_heredocs, int *count_heredocs)
{
	t_token *cur;
	char *heredoc_eof;
	t_list *node;
	char *input_file;
	char *output_file;
	int ret;
	
	(void)bin;
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
				return handle_errno(input_file, 1, NULL, NULL);
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
					return handle_errno(output_file, 1, NULL, NULL);
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
					return handle_errno(output_file, 1, NULL, NULL);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, "<<") == 0)
		{
			heredoc_eof = ft_strdup(cur->next->content);
			if (!heredoc_eof)
				return (1);
			node = ft_lstnew(heredoc_eof);
			if (!node)
			{
				free(heredoc_eof);
				return (1);
			}
			ft_lstadd_back(heredoc_eofs, node);
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			*count_heredocs = *count_heredocs + 1;
		}
		cur = cur->next;
	}
	if ((*count_heredocs > 0) && (*count_heredocs == nb_heredocs))
	{
		if (redir->input_redir != -1)
			close(redir->input_redir);
		ret = multiple_heredoc(*heredoc_eofs, &(redir->input_redir), *count_heredocs);
		ft_garbage(heredoc_eofs);
		if (ret != 0)
			return ret;
	}
	return 0;
}
