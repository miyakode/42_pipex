/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_children.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <xxxxxxxx@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:02:09 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2024/12/04 19:01:23 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	child_proc(t_job *job, char **envp, int flag)
{
	if (flag != FIRST_CHILD)
		close(job->fd1[1]);
	close(job->fd2[0]);
	if (flag == FIRST_CHILD && !(job->heredoc))
		open_file(job, 1);
	else if (!(flag == FIRST_CHILD && job->heredoc))
	{
		if (dup2(job->fd1[0], 0) == -1)
			err_exit(job, NULL, NULL);
		close(job->fd1[0]);
	}
	if (flag == LAST_CHILD)
		open_file(job, 0);
	else if (!(flag == FIRST_CHILD && job->heredoc))
		if (dup2(job->fd2[1], 1) == -1)
			err_exit(job, NULL, NULL);
	if (!(flag == FIRST_CHILD && job->heredoc))
		close(job->fd2[1]);
	if (flag == FIRST_CHILD && job->heredoc)
		get_heredoc(job);
	check_cmd(job);
	if (execve(job->cmd_path, job->cmd, envp))
		err_exit(job, NULL, NULL);
}

void	open_file(t_job *job, int isinfile)
{
	int	fd;

	if (isinfile)
		fd = open(job->filename[0], O_RDONLY);
	else if (job->heredoc)
		fd = open(job->filename[1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(job->filename[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
		close(job->fd2[1]);
		if (isinfile)
			err_exit(job, NULL, job->filename[0]);
		err_exit(job, NULL, NULL);
	}
	if (!job->heredoc && isinfile && dup2(fd, 0) == -1)
		err_exit(job, NULL, NULL);
	else if (!isinfile && dup2(fd, 1) == -1)
		err_exit(job, NULL, NULL);
	close(fd);
}

void	get_heredoc(t_job *job)
{
	char	*s;

	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", 2);
		s = get_next_line(0);
		if (!s)
			err_exit(job, NULL, NULL);
		if (!ft_strncmp(s, job->limiter, ft_strlen(job->limiter) + 1))
		{
			free(s);
			break ;
		}
		if (**(job->cmd_arg) && write(job->fd2[1], s, ft_strlen(s)) == -1)
		{
			free(s);
			err_exit(job, NULL, NULL);
		}
		free(s);
	}
	free(job->limiter);
	close(job->fd2[1]);
	exit(0);
}

void	check_cmd(t_job *job)
{
	char	**path;

	parse_cmd(job);
	path = job->path;
	while (*path)
	{
		job->cmd_path = ft_strjoin(*path++, job->cmd[0]);
		if (!(job->cmd_path))
			err_exit(job, NULL, NULL);
		if (!access(job->cmd_path, F_OK | X_OK))
			return ;
		else
		{
			free(job->cmd_path);
			job->cmd_path = NULL;
		}
	}
	if (!access(job->cmd[0], F_OK | X_OK))
	{
		job->cmd_path = job->cmd[0];
		job->cmd[0] = ft_strrchr(job->cmd[0], '/') + 1;
		return ;
	}
	err_exit(job, ERR_CMD, job->cmd[0]);
}

void	parse_cmd(t_job *job)
{
	if (job->cmd && job->cmd[0] != (ft_strrchr(job->cmd_path, '/') + 1))
	{
		free_strarr(job->cmd);
		free(job->cmd);
	}
	job->cmd = ft_split(*(job->cmd_arg), ' ');
	if (!(job->cmd))
		err_exit(job, NULL, NULL);
	if (!*(job->cmd))
		err_exit(job, ERR_EMP, NULL);
}
