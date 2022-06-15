/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 10:50:48 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/15 12:30:46 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_nb_cmd(t_token *commands)
{
	int count;

	count = 1;
	while (commands)
	{
		if (commands->type == 1 && (ft_strcmp(commands->content, "|") == 0))
			count++;
		commands = commands->next;
	}
	return count;
}

t_token **split_commands(t_token *commands, int nb_cmd)
{
	int i;
	t_token *tmp;
	
	t_token **cmd_table = malloc(sizeof(t_token *) * (nb_cmd + 1));
	if (!cmd_table)
		return NULL;
	i = 0;
	while (i < nb_cmd)
	{
		cmd_table[i] = NULL;
		i++;
	}
	i = 0;
	while (commands)
	{
		if (commands->type == 1 && (ft_strcmp(commands->content, "|") == 0))
			i++;
		if (commands->type == 2)
		{
			tmp = commands;
			commands = commands->next;
			tmp->next = NULL;
			ft_lstadd_back_token(&cmd_table[i], tmp);
		}
		else
			commands = commands->next;
	}
	return cmd_table;
}


int find_input_and_output_files(t_token **commands, t_redir *redir)
{
	t_token *cur;
	char *input_file;
	char *output_file;
	char * heredoc_eof;
	
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
				return handle_errno(input_file, -1, NULL, NULL);
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
					return handle_errno(output_file, -1, NULL, NULL);
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
					return handle_errno(output_file, -1, NULL, NULL);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, "<<") == 0)
		{
			if (redir->input_redir != -1)
				close(redir->input_redir);
			heredoc_eof = cur->next->content;
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			redir->input_redir = heredoc(heredoc_eof);
			if (redir->input_redir == -1)
				return -1;
		}
		cur = cur->next;
	}
	return 0;
}
