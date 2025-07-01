/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <xxxxxxxx@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:00:33 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2024/12/04 19:21:04 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	get_path(t_job *job, char **envp)
{
	char	*p;
	int		i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH", 4))
		i++;
	job->path = ft_split(envp[i] + 5, ':');
	if (!(job->path))
		err_exit(job, NULL, NULL);
	i = -1;
	while (job->path[++i])
	{
		p = job->path[i];
		job->path[i] = NULL;
		job->path[i] = ft_strjoin(p, "/");
		free(p);
		if (!job->path[i])
			err_exit(job, NULL, NULL);
	}
}

void	err_exit(t_job *job, char *str_err, char *name)
{
	if (str_err)
	{
		write(2, "pipex: ", 7);
		ft_putstr_fd(str_err, 2);
	}
	else
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
	ft_putstr_fd(": ", 2);
	if (name)
		ft_putstr_fd(name, 2);
	write(2, "\n", 1);
	if (job)
		free_jobdata(job);
	if (str_err)
		exit(127);
	exit(errno);
}

void	free_jobdata(t_job *job)
{
	if (job->path)
	{
		free_strarr(job->path);
		free(job->path);
	}
	if (job->cmd)
	{
		free_strarr(job->cmd);
		free(job->cmd);
	}
	if (job->cmd_path)
		free(job->cmd_path);
	if (job->limiter)
		free(job->limiter);
}

void	free_strarr(char **s)
{
	while (*s)
		free(*s++);
}

void	print_usage(void)
{
	ft_putendl_fd(\
	"\n\n\t************************ PIPEX **************************", 1);
	ft_putendl_fd("\n\t$ ./pipex infile \"cmd1\" \"cmd2\" outfile", 1);
	ft_putendl_fd("\n\tbehave like:", 1);
	ft_putendl_fd("\t$ infile cmd1 | cmd2 > outfile", 1);
	ft_putendl_fd("\n\n\tFor more commands and heredoc:", 1);
	ft_putendl_fd("\n\t$ make bonus", 1);
	ft_putendl_fd("\n\t$ ./pipex infile \"cmd1\" \"cmd2\" ... \"cmdN\" outfile", \
	1);
	ft_putendl_fd("\n\tor", 1);
	ft_putendl_fd("\n\t$ ./pipex here_doc LIMITER \"cmd1\" \"cmd2\" outfile", \
	1);
	ft_putendl_fd("\n\tbehave like:", 1);
	ft_putendl_fd("\t$ cmd1 << LIMITER | cmd2 >> outfile", 1);
	ft_putendl_fd(\
	"\n\n\t*********************************************************\n\n", 1);
}
