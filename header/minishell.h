/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:05:03 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/16 11:31:22 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <../libft/libft.h>
# include <errno.h>
# include <fcntl.h>

#include <dirent.h>

# define BUFFER_SIZE 4096

typedef struct s_token
{
	int				type; //1-operator, 2-word, 3-ponct, 4-space, 5-redir
	char			*content;
	struct s_token	*next;
}	t_token;

typedef struct s_vars
{
	t_token	**cmd;
	char	***env;
	t_list	**bin;
	pid_t	pid; 
	char	*cmd_line;
}	t_vars;

typedef struct s_redir
{
	int		tmpin;
	int		tmpout;
	int		fdin;
	int		fdout;
	int		input_redir;
	int		output_redir;
	int		fdpipe[2];
	t_token	**cmd_table;
	int		nb_cmd;
	int		ret;
	int		i;
}	t_redir;

extern int g_exit_status;

//	main.c
int	main(int argc, char **argv, char **env);
int search_cmd(t_vars *vars);
void	ft_print(t_token *token);
int	ft_strcmp(char *s1, char *s2);
int echo(t_token *command);
int handle_error(char *error_msg, int ret_value, char *to_free1, char *to_free2);
int pwd();
int cd(t_token *command, char ***env);
int export(t_token *command, char ***env);
int env_builtin(char **env);
void free_strs_array(char **strs);
char *get_env_value(char *key, char **env);
int unset(t_token *command, char ***env);
void	ft_lstadd_back_token(t_token **alst, t_token *new);
int redir_and_exec(t_vars *vars);
void ft_delete_token(t_token **alst, t_token *to_del);
void free_strs(char **strs);
char **dup_env(char **envp);
void handle_sigquit(int code);
void handle_sigint(int code);
void handle_sigint_no_prompt(int code);
int	handle_errno(char *error_msg, int ret, t_token **to_free1, t_list **to_free2);
int exit_builtin(t_token *command, char ***env, t_list **bin, char *cmd_line);
void	ft_garbage(t_list **bin);
int add_var_to_env(char *s, char ***env);
int set_var_in_env(char *s, char *var_name, int name_len, char ***env);
int	print_sorted_env(char **env);
char	*get_var_name(char *s, int name_len);
int	var_name_is_valid(char *s, int *name_len);
char	**cmd_to_strs(t_token *command);
int	exec_cmd(t_token *command, char **env, pid_t pid);
int	heredoc(char *heredoc_eof, int *input_redir);
int	multiple_heredoc(t_list *heredoc_eofs, int *input_redir, int nb_heredocs);
int find_input_and_output_files(t_token **commands, t_redir *redir, t_list **bin, t_list **heredoc_eofs, int nb_heredocs, int *count_heredocs);
int	get_nb_cmd_and_heredocs(t_token *commands, int *nb_heredocs);
t_token **split_commands(t_token *commands, int nb_cmd, int i);
int	get_child_status(int pid, int *ret);
void	ft_delete_node(t_list **alst, t_list *to_del);

#endif