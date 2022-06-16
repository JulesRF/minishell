/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 09:31:52 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/16 12:11:26 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Set the initial input for execution to the input file if there is one.
 * If not, the input is set to default.
 * @param vars variables related to command execution
 * @param redir variables related to redirections
 * @return int 1 if error, 0 otherwise
 */
int	set_init_input(t_vars *vars, t_redir *redir)
{
	(void)vars;
	if (redir->input_redir != -1)
		redir->fdin = redir->input_redir;
	else if (redir->i == 0)
	{
		redir->fdin = dup(redir->tmpin);
		if (redir->fdin == -1)
			return (handle_errno("dup", 1, NULL, NULL));
	}
	if (dup2(redir->fdin, 0) == -1)
		return (handle_errno("dup2", 1, redir->cmd_table, NULL));
	close(redir->fdin);
	return (0);
}

/**
 * @brief Set the output to be the output file if there is one) or the inital
 * output if this is the last command. If not, set the output to the write end
 * side of a pipe, and the input to the read end side of it.
 * @param redir variables related to redirections
 * @return int 1 if error, should not return otherwise
 */
int	set_output(t_redir *redir)
{	
	if (redir->i == (redir->nb_cmd - 1) && redir->output_redir == -1)
	{
		redir->fdout = dup(redir->tmpout);
		if (redir->fdout == -1)
			return (handle_errno("dup", 1, redir->cmd_table, NULL));
	}
	else
	{
		if (pipe(redir->fdpipe) == -1)
			return (handle_errno("pipe", 1, redir->cmd_table, NULL));
		redir->fdin = (redir->fdpipe)[0];
		redir->fdout = (redir->fdpipe)[1];
		
		if (redir->output_redir != -1)
		{
			close(redir->fdout);
			redir->fdout = redir->output_redir;
		}
	}
	if (dup2(redir->fdout, 1) == -1)
		return (handle_errno("dup2", 1, redir->cmd_table, NULL));
	close(redir->fdout);
	return (0);
}

/**
 * @brief Fork the process to execute the command
 * 
 * @param vars variables related to command execution
 * @param redir variables related to redirections
 * @return int 1 if error, should not return otherwise
 */
int	fork_exec(t_vars *vars, t_redir *redir)
{
	vars->pid = fork();
	if (vars->pid == -1)
		return (handle_errno("fork failed", 1, redir->cmd_table, NULL));
	if (vars->pid == 0)
	{
		close(redir->fdin);
		redir->ret = search_cmd(vars);
		exit(redir->ret);
	}
	return (0);
}

/**
 * @brief Restore default in/out fd of parent process
 * and wait for child processes to die
 * @param vars variables related to command execution
 * @param redir variables related to redirections
 * @return int 
 */
int	restore_in_out_and_wait(t_vars *vars, t_redir *redir)
{
	int	wait_ret;

	free(redir->cmd_table);
	if (dup2(redir->tmpin, 0) == -1)
		return (handle_errno("dup2", 1, NULL, NULL));
	if (dup2(redir->tmpout, 1) == -1)
		return (handle_errno("dup2", 1, NULL, NULL));
	close(redir->tmpin);
	close(redir->tmpout);
	if (redir->nb_cmd > 1)
	{
		if (redir->ret == 0)
			get_child_status(vars->pid, &(redir->ret));
		wait_ret = 0;
		while (wait_ret != -1)
			wait_ret = waitpid(-1, &(redir->i), 0);
	}
	return (0);
}

/**
 * @brief Find the input/output files if there are any and
 * if there are any pipes, launch each command in a child process
 * and connect their output/input accordingly.
 * @param vars variables related to command execution
 * @return int 1 if error, the exit status of the last command otherwise
 */
int	redir_and_exec(t_vars *vars)
{
	t_redir	redir;
	t_list *heredoc_eofs;
	int nb_heredocs;
	int count_heredocs;

	redir.tmpin = dup(0);
	redir.tmpout = dup(1);
	if (redir.tmpin == -1 || redir.tmpout == -1)
		return (handle_errno("dup", 1, NULL, NULL));

	redir.nb_cmd = get_nb_cmd_and_heredocs(*(vars->cmd), &nb_heredocs);
	redir.cmd_table = split_commands(*(vars->cmd), redir.nb_cmd, redir.i);
	if (!(redir.cmd_table))
		return (1);
	redir.ret = 0;
	redir.i = -1;
	heredoc_eofs = NULL;
	count_heredocs = 0;
	// for(int i = 0; i < redir.nb_cmd; i++)
	// {
	// 	printf("cmd_table[%d]:\n", i);
	// 	ft_print(redir.cmd_table[i]);
	// }

	while (++(redir.i) < redir.nb_cmd)
	{
		redir.ret = find_input_and_output_files(&((redir.cmd_table)[redir.i]), &redir, vars->bin, &heredoc_eofs, nb_heredocs, &count_heredocs);
		// if (redir.ret != 0)
		// 	continue ;
		if (set_init_input(vars, &redir) == 1)
			redir.ret = 1;
		if (set_output(&redir) == 1)
			redir.ret = 1;
		vars->pid = -1;
		vars->cmd = &((redir.cmd_table)[redir.i]);
		if (redir.nb_cmd > 1 && redir.ret == 0)
		{
			if (fork_exec(vars, &redir) == 1)
				return (1);
		}
		else if (redir.ret == 0)
			redir.ret = search_cmd(vars);
	}
	restore_in_out_and_wait(vars, &redir);
	return (redir.ret);
}
