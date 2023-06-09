/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 22:05:13 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/08 16:36:15 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_param
{
	int				n_philo;
	int				eat_time;
	int				sleep_time;
	int				die_time;
	int				must_eat;
	int				st_time;
	int				end_of_simulation;
	pthread_mutex_t	gard_end;
	pthread_mutex_t	gard_n_eat;
	pthread_mutex_t	gard_alive;
	pthread_mutex_t	*mutex;
}			t_param;

typedef struct s_data
{
	int			id;
	int			alive_time;
	int			n_eat;
	int			philo_must_eat;
	pthread_t	philo;
	t_param		*param;
}			t_data;

int			ft_atoi(const char *str);
void		*ft_calloc(size_t count, size_t size);
int			get_current_time(void);
void		my_usleep(int time_to_sleep, t_data *data);
void		control_simulation(t_data *data, t_param *param);
void		start_simulation(t_data *data, t_param *param);
int			print_help(void);
int			check_error(char *data);
t_param		*get_philo_param(int ac, char **av);

#endif