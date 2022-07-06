/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:10:11 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/23 08:48:05 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

int	increment_shlvl(char ***env)
{
	char	*shlvl_str;
	char	*shlvl_var;
	int		shlvl;

	shlvl_str = get_env_value("SHLVL", *env);
	if (!shlvl_str)
		return (1);
	shlvl = ft_atoi(shlvl_str);
	free(shlvl_str);
	shlvl++;
	if (shlvl < 0)
		shlvl = 0;
	else if (shlvl >= 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		ft_putnbr_fd(shlvl, 2);
		ft_putendl_fd(") too high, resetting to 1", 2);
		shlvl = 1;
	}
	shlvl_str = ft_itoa(shlvl);
	if (!shlvl_str)
		return (1);
	shlvl_var = ft_strjoin("SHLVL=", shlvl_str);
	free(shlvl_str);
	if (!shlvl_var)
		return (1);
	shlvl = set_var_in_env(shlvl_var, "SHLVL", 5, env);
	free(shlvl_var);
	return (shlvl);
}

void	ft_initvars(t_token **token, t_data *data, t_list **bin, t_vars *vars)
{
	vars->cmd = token;
	vars->env = data->env;
	vars->bin = bin;
	vars->cmd_line = data->cmd_line;
}

t_data	*ft_initdata(char ***env)
{
	t_data	*tmp;

	tmp = malloc(sizeof(t_data));
	if (!tmp)
	{
		free_strs_array(*env);
		return (NULL);
	}
	tmp->env = env;
	return (tmp);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*bin;
	t_token	*token;
	t_data	*data;
	char	**env;

	g_exit_status = 0;
	ft_preparse(argc, argv, envp);
	bin = NULL;
	token = NULL;
	env = dup_env(envp);
	if (!env)
		exit(1);
	if (increment_shlvl(&env) == 1)
	{
		free_strs_array(env);
		exit(1);
	}
	data = ft_initdata(&env);
	if (!data)
		exit(1);
	ft_prompt(&token, &bin, data);
	free_strs_array(env);
	free (data);
	ft_garbage(&bin);
	exit(g_exit_status);
}
