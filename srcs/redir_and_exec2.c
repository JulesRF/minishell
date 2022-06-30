/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_and_exec2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 10:53:01 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/29 11:02:22 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_input_and_output(t_redir *redir, t_vars *vars)
{
	redir->ret = find_in_out_files(&((redir->cmd_table)[redir->i]), redir);
	if (set_input(redir) == 1 && redir->ret != 2)
		redir->ret = 1;
	if (set_output(redir) == 1 && redir->ret != 2)
		redir->ret = 1;
	vars->pid = -1;
	vars->cmd = &((redir->cmd_table)[redir->i]);
}
