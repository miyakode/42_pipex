/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <xxxxxxxx@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:57:25 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2024/12/04 19:00:59 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# include "libft.h"

# define ERR -1
# define ERR_CMD "command not found"
# define ERR_EMP "permission denied"
# define FIRST_CHILD 1
# define LAST_CHILD 2

typedef struct s_job
{
	int		prc_count;
	int		fd1[2];
	int		fd2[2];
	int		heredoc;
	char	*limiter;
	char	**path;
	char	**cmd;
	char	*cmd_path;
	char	**cmd_arg;
	char	*filename[2];
	int		exit_code;
}	t_job;

void	parse_args(t_job *job, int ac, char **av, char **envp);
void	create_child(t_job *job, char **envp, int flag);
void	set_pipe(t_job *job);
void	parent_proc(t_job *job, char **envp, int id, int flag);

void	child_proc(t_job *job, char **envp, int flag);
void	open_file(t_job *job, int isinfile);
void	get_heredoc(t_job *job);
void	check_cmd(t_job *job);
void	parse_cmd(t_job *job);

void	get_path(t_job *job, char **envp);
void	free_strarr(char **s);
void	free_jobdata(t_job *job);
void	err_exit(t_job *job, char *str_err, char *name);
void	print_usage(void);
#endif
