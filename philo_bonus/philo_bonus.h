/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:40:31 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/09 15:49:53 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

typedef struct s_param
{
	int				n_philo;
	int				eat_time;
	int				sleep_time;
	int				die_time;
	int				must_eat;
	int				st_time;
	int				end_of_simulation;
	sem_t			*gard_end;
	sem_t			*gard_n_eat;
	sem_t			*gard_alive;
	sem_t			*gard_must;
	sem_t			*sem;
	
}			t_param;

typedef	struct s_data
{
	int			id;
	int			alive_time;
	int			n_eat;
	int			stop;
	pid_t		philo;
	t_param		*param;
	pthread_t	control;
}			t_data;

int			ft_atoi(const char *str);
void		*ft_calloc(size_t count, size_t size);
int			get_current_time(void);
void		my_usleep(int time_to_sleep, t_data *data);
int			print_help(void);
int			check_error(char *data);
t_param		*get_philo_param(int ac, char **av);

#endif
