/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 10:41:30 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/05 18:25:18 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc(int pipe_fd[2], char *line, t_vars *vars)
{
	t_list	*bin;
	t_data	data;
	t_token	*token;
	char *new_line;

	bin = NULL;
	token = NULL;
	data.cmd_line = line;
	data.env = vars->env;
	ft_token(&token, &bin, &data);
	ft_questionmark(token, &bin, &data);
	ft_sepdollar(token, &bin, &data);
	ft_supempty(&token);
	ft_dollar(token, &bin, &data);       
	new_line = cmd_to_str(token);  
	if (!new_line)
	{
		ft_garbage(&bin);
		return ;
	}
	write(pipe_fd[1], new_line, ft_strlen(new_line));
	write(pipe_fd[1], "\n", 1);
	free(new_line);
	ft_garbage(&bin);
}
