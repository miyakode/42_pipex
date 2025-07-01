/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <xxxxxxxx@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:47:31 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2024/12/04 17:58:56 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int ac, char **av, char **envp)
{
	t_job	job;
	int		exit_code;

	if (ac < 5)
		return (print_usage(), 1);
	ft_bzero(&job, sizeof(t_job));
	if (!ft_strncmp(av[1], "here_doc", 9))
	{
		if (ac < 6)
			return (print_usage(), 1);
		job.heredoc = 1;
	}
	parse_args(&job, ac, av, envp);
	create_child(&job, envp, FIRST_CHILD);
	exit_code = job.exit_code >> 8;
	free_jobdata(&job);
	exit(exit_code);
}

void	parse_args(t_job *job, int ac, char **av, char **envp)
{
	int	idx;

	idx = 1;
	if (job->heredoc)
		idx++;
	job->prc_count = ac - 3;
	if (job->heredoc)
	{
		job->limiter = ft_strjoin(av[idx++], "\n");
		if (!(job->limiter))
			err_exit(job, NULL, NULL);
	}
	else
		job->filename[0] = av[idx++];
	job->filename[1] = av[ac - 1];
	job->cmd_arg = av + idx;
	get_path(job, envp);
	job->fd2[0] = -1;
	job->fd2[1] = -1;
}

void	create_child(t_job *job, char **envp, int flag)
{
	int	id;

	set_pipe(job);
	id = fork();
	if (id == -1)
		err_exit(job, NULL, NULL);
	if (!id)
		child_proc(job, envp, flag);
	else
		parent_proc(job, envp, id, flag);
}

void	set_pipe(t_job *job)
{
	job->fd1[0] = job->fd2[0];
	job->fd1[1] = job->fd2[1];
	if (pipe(job->fd2) == -1)
		err_exit(job, NULL, NULL);
}

void	parent_proc(t_job *job, char **envp, int id, int flag)
{
	if (flag != FIRST_CHILD)
	{
		close(job->fd1[0]);
		close(job->fd1[1]);
	}
	if (!(job->heredoc && flag == FIRST_CHILD))
		job->cmd_arg++;
	if (--job->prc_count == 1)
		create_child(job, envp, LAST_CHILD);
	else if (job->prc_count)
		create_child(job, envp, 0);
	if (!job->prc_count--)
	{
		close(job->fd2[0]);
		close(job->fd2[1]);
		waitpid(id, &(job->exit_code), 0);
	}
	else
		waitpid(id, NULL, 0);
}
