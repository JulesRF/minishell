/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:05:03 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/07/13 18:04:48 by vfiszbin         ###   ########.fr       */
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
# include <dirent.h>

# define BUFFER_SIZE 4096

//1-operator, 2-word, 3-ponct, 4-space, 5-redir, 6-phantom
typedef struct s_token
{
	int				type;
	int				qt;
	char			*content;
	struct s_token	*next;
}	t_token;

typedef struct s_data {
	char	*cmd_line;
	char	***env;
	t_token	*temp;
	t_token	*stop;
	t_token	**cmd;
	t_list	**bin;
	pid_t	pid;
	int		i;
	int		j;
}	t_data;

typedef struct s_redir
{
	int		tmpin;
	int		tmpout;
	int		fdin;
	int		fdout;
	int		input_redir;
	int		heredoc_redir;
	int		output_redir;
	int		fdpipe[2];
	t_token	**cmd_table;
	int		nb_cmd;
	int		ret;
	int		i;
	t_list	*heredoc_eofs;
	int		nb_heredocs;
	int		count_heredocs;
	int		missing_heredoc_delim;
}	t_redir;

extern int	g_exit_status;

//	main.c
void	ft_initvars(t_token **token, t_data *data, t_list **bin);
int		main(int argc, char **argv, char **env);

//	redir_and_exec.c
int		redir_and_exec(t_data *vars);
int		set_input(t_redir *redir);
int		set_output(t_redir *redir);

//	redir_and_exec2.c
void	set_input_and_output(t_redir *redir, t_data *vars);

//	heredoc.c
int		multiple_heredoc(t_redir *redir, t_data *vars);
int		find_heredocs(t_token **commands, t_redir *redir, t_data *vars);

//	heredoc2.c
void	write_heredoc(int pipe_fd[2], char *line, t_data *vars,
			int quoted_delim);
void	clean_and_close_heredoc(int *pipe_fd, t_data *vars, t_redir *redir);
int		close_pipe_and_ret(int *pipe_fd, int ret);

//	redirections.c
int		find_in_out_files(t_token **commands, t_redir *redir);

//	execution.c
int		exec_cmd(t_data *vars, pid_t pid, t_redir *redir);
int		get_child_status(int pid, int *ret, int change_sig, int ignore_err);
int		fork_exec(t_data *vars, t_redir *redir);

//	search_cmd.c
int		search_cmd(t_data *vars, t_redir *redir);

//	signal.c
void	handle_sigquit(int code);
void	handle_sigint(int code);
void	handle_sigint_no_prompt(int code);
void	handle_sigquit_heredoc(int code);
void	change_signals(int i);
//	signal2.c
void	handle_sigpipe(int code);

//	utils
char	*get_env_value(char *key, char **env);
int		handle_error(char *error_msg, int ret_value, char *to_free1,
			char *to_free2);
int		handle_errno(char *error_msg, int ret, t_token **to_free1,
			t_list **to_free2);
char	*get_env_var(char *key, char **env);

//	utils2.c
char	**cmd_to_strs(t_token *command);
char	*cmd_to_str(t_token *command);
void	free_strs(char **strs);
int		get_nb_cmd_and_heredocs(t_token *commands, int *nb_heredocs);
t_token	**split_commands(t_token *commands, int nb_cmd, int i);

//	garbage.c
void	ft_garbage(t_list **bin);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_backs(t_list **alst, t_list *new, t_data *data);

//	token_utils.c
t_token	*ft_lstnew_token(t_list **bin, t_data *data, char *content, int type);
t_token	*ft_lstlast_token(t_token *lst);
void	ft_lstadd_back_token(t_token **alst, t_token *new, t_data *data);
void	ft_delete_token(t_token **alst, t_token *to_del);
void	ft_clean_token(t_token **token);

//	parse_utils.c
char	*ft_strdup(const char *s1);
int		ft_strcmp(char *s1, char *s2);
void	ft_preparse(int argc, char **argv, char **env);
void	ft_print(t_token *token);
int		ft_is_quotes(t_token *token);

//	token.c
void	ft_parse_operator(t_token **token, t_list **bin, t_data *data, int i);
void	ft_parse_ponct(t_token **token, t_list **bin, t_data *data, int i);
int		ft_parse_redir(t_token **token, t_list **bin, t_data *data, int i);
int		ft_parse_word(t_token **token, t_list **bin, t_data *data, char *str);
void	ft_token(t_token **token, t_list **bin, t_data *data);

//	quotes.c
t_token	*ft_joincontent(t_token *temp, t_token *token, t_list **bin,
			t_data *data);
void	ft_doublequotes(t_token *token, t_list **bin, t_data *data,
			t_token *temp);
void	ft_simplequotes(t_token *token, t_list **bin, t_data *data,
			t_token *temp);
void	ft_quotesbool(t_token *token, t_data *data);

//	dollar.c
void	ft_dollarfind(t_token *token, char *to_find, t_data *data,
			t_list **bin);
t_token	*ft_isdollar(t_token *token, t_list **bin, t_data *data, int inf);
void	ft_dollar(t_token *token, t_list **bin, t_data *data);
t_token	*ft_splitdollar(t_token *token, t_list **bin, int i, t_data *data);
void	ft_sepdollar(t_token *token, t_list **bin, t_data *data);

//	dollar2.c
int		ft_dollarcheck(t_token *token, char *to_find, char **env, t_list **bin);
void	ft_dollardoc(t_token *token, t_list **bin, t_data *data);

//	simplify_utils2.c
void	ft_handleheredoc(t_token *token, t_list **bin, t_data *data);
int		ft_add(t_token *token);
void	ft_ghostbuster(t_token **token, t_list **bin, t_data *data);

//	simplify.c
int		ft_piperedir(t_token *token);
void	ft_rmvquotes(t_token **token, t_list **bin);
void	ft_joinwords(t_token **token, t_list **bin, t_data *data);
int		ft_first_quote(t_token *token);
int		ft_simplify(t_token **token, t_list **bin, t_data *data);

//	prompt.c
int		ft_closed_quotes(char *str, int i);
int		ft_syntax(char *str, t_list **bin);
void	ft_prompt(t_token **token, t_list **bin, t_data *data);
char	**dup_env(char **envp);
void	free_strs_array(char **strs);

//	questionmark.c
void	ft_supempty(t_token **token);
void	ft_questionmark(t_token *token, t_list **bin, t_data *data);

//	simplify_utils.c
void	ft_supspace(t_token **token);
void	ft_fucknorm(t_token *stop, char *str, t_list **bin, t_data *data);
void	ft_getmeout(t_data *data);
t_token	*ft_addempty(t_token *token, t_token *stop, t_list **bin, t_data *data);

////	builtins	////
//	echo.c
int		echo(t_token *command);

//	cd.c
int		cd(t_token *command, char ***env);

//	cd2.c
int		unset_oldpwd(char ***env);
int		get_current_pwd(char **new_str);
int		path_exists(char *path);
int		set_new_dir(char **path, char **old_pwd, char **env,
			int path_allocated);

//	pwd.c
int		pwd(void);

//	export.c
int		export(t_token *command, char ***env);
int		add_var_to_env(char *s, char ***env);
int		set_var_in_env(char *s, char *var_name, int name_len, char ***env);

//	export2.c
int		print_sorted_env(char **env);
char	*get_var_name(char *s, int name_len);
int		var_name_is_valid(char *s, int *name_len);

//export3.c
int		concat_to_env(t_token *command, char ***env, int name_len);

//	env.c
int		env_builtin(char **env);

//	exit.c
void	clean_prog(t_data *data, t_redir *redir);
void	exit_prog(char *msg, int exit_status, t_redir *redir);
int		exit_builtin(t_token *command, t_data *data, t_redir *redir);

//	unset.c
int		unset(t_token *command, char ***env);
int		rm_var_from_env(char *s, char ***env);
////	////	////

#endif