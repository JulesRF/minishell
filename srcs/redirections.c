/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 10:50:48 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/14 17:18:13 by vfiszbin         ###   ########.fr       */
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

void	write_heredoc(int pipe_fd[2], char *line)
{
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
}

int heredoc(char *heredoc_eof)
{
	char	*line;
	int		pipe_fd[2];
	int		output_redir;

	if (pipe(pipe_fd) == -1)
		return handle_errno("dup",-1, NULL, NULL);
	while (1)
	{
		line = readline("> ");
		if (line && (ft_strcmp(line, heredoc_eof) == 0))
			break ;
		//line = dollar substitution ?
		write_heredoc(pipe_fd, line);
		free (line);
	}
	free(line);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	output_redir = dup(0);
	if (output_redir == -1)
		return handle_errno("dup", -1, NULL, NULL);
	return (output_redir);
}


int find_input_and_output_files(t_token **commands, int *input_redir, int *output_redir)
{
	t_token *cur;
	char *input_file;
	char *output_file;
	char * heredoc_eof;
	
	*input_redir = -1;
	*output_redir = -1;
	cur = *commands;
	while (cur)
	{
		if (cur->type == 5 && ft_strcmp(cur->content, "<") == 0)
		{
			if (*input_redir != -1)
				close(*input_redir);
			input_file = cur->next->content; //check if next NULL ? devrait pas l'etre apres parsing
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			*input_redir = open(input_file, O_RDONLY);
			if (*input_redir == -1)
				return handle_errno(input_file, -1, NULL, NULL);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, ">") == 0)
		{
			if (*output_redir != -1)
				close(*output_redir);
			output_file = cur->next->content; //check if next NULL ? devrait pas l'etre apres parsing
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			*output_redir = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			if (*output_redir == -1)
					return handle_errno(output_file, -1, NULL, NULL);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, ">>") == 0)
		{
			if (*output_redir != -1)
				close(*output_redir);
			output_file = cur->next->content; //check if next NULL ? devrait pas l'etre apres parsing
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			*output_redir = open(output_file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
			if (*output_redir == -1)
					return handle_errno(output_file, -1, NULL, NULL);
		}
		else if (cur->type == 5 && ft_strcmp(cur->content, "<<") == 0)
		{
			if (*input_redir != -1)
				close(*input_redir);
			heredoc_eof = cur->next->content;
			ft_delete_token(commands, cur->next);
			ft_delete_token(commands, cur);
			*input_redir = heredoc(heredoc_eof);
			if (*input_redir == -1)
				return -1;
		}
		cur = cur->next;
	}
	return 0;
}



int redir_and_exec(t_vars *vars)
{
	int tmpin;
	int tmpout;
	int fdin;
	int fdout;
	int input_redir;
	int output_redir;
	int nb_cmd;
	int ret;
	pid_t pid;
	int i;
	t_token **cmd_table;
	int fdpipe[2];
	int status;

	//store default in/out fd of parent process for later reset
	tmpin = dup(0);
	tmpout = dup(1);
	if (tmpin == -1 || tmpout == -1)
		return handle_errno("dup", 1, NULL, NULL);

	if (find_input_and_output_files(vars->cmd, &input_redir, &output_redir) == -1)
		return (1);

	//set initial input
	if (input_redir != -1)
		fdin = input_redir;
	else
	{
		fdin = dup(tmpin); //default in
		if (fdin == -1)
			return handle_errno("dup", 1, NULL, NULL);
	}
		
	nb_cmd = get_nb_cmd(*(vars->cmd));
	cmd_table = split_commands(*(vars->cmd), nb_cmd);
	if (!cmd_table)
		return (1);
	ret = 0;
	i = 0;
	while (i < nb_cmd)
	{
		//redirect input to fdin
		if (dup2(fdin, 0) == -1)
			return handle_errno("dup2", 1, cmd_table, NULL); //éviter de sortir de la boucle ?
		close(fdin); //bc not needed yet

		//set output
		if (i == nb_cmd - 1) //last cmd
		{
			if (output_redir != -1)
				fdout = output_redir;
			else
			{
				fdout = dup(tmpout); //default out
				if (fdout == -1)
					return handle_errno("dup", 1, cmd_table, NULL); //éviter de sortir de la boucle ?
			}
		}
		else //not last cmd
		{
			//create pipe
			if (pipe(fdpipe) == -1)
				return handle_errno("pipe", 1, cmd_table, NULL); //éviter de sortir de la boucle ?
			fdin = fdpipe[0]; //sera l'input lors de la prochaine itération
			fdout = fdpipe[1];
		}

		
		//redirect ouput to fdout
		if (dup2(fdout, 1) == -1)
			return handle_errno("dup2", 1, cmd_table, NULL); //éviter de sortir de la boucle ?
		close(fdout);

		pid = -1;
		if (nb_cmd > 1) //if there are pipes
		{
			pid = fork();
			if (pid == -1) //fork failed
				return handle_errno("fork failed", 1, cmd_table, NULL);
			if (pid==0)
			{
				//g_exit_status
				close(fdin);
				vars->cmd = &(cmd_table[i]);
				vars->pid = pid;
				ret = search_cmd(vars);
				exit(ret);
			}
		}
		else
		{
			vars->cmd = &(cmd_table[i]);
			vars->pid = pid;
			ret = search_cmd(vars);
		}
		
		i++;
	}
	
	free(cmd_table);

	//restore default in/out fd of parent process
	if (dup2(tmpin, 0) == -1)
		return handle_errno("dup2", 1, NULL, NULL);
	if (dup2(tmpout, 1) == -1)
		return handle_errno("dup2", 1, NULL, NULL);
	close(tmpin);
	close(tmpout);
	
	if (nb_cmd > 1)
	{
		signal(SIGINT, handle_sigint_no_prompt);
		if (waitpid(pid, &status, 0) == -1)
			return handle_errno("waitpid", 1, NULL, NULL);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);

		if (WIFSIGNALED(status))
		{
			ret = WTERMSIG(status);
			ret += 128;
		}
		signal(SIGINT, handle_sigint);
		while (waitpid(-1, &status, 0) != -1);		
	}

	return ret;
}