/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starting_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:18:22 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/12 18:31:33 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	must_eat(t_data *data)
{
	if (data->param->must_eat != -1 && data->stop == 0)
	{
		sem_wait(data->param->gard_n_eat);
		if (data->n_eat >= data->param->must_eat)
		{
			sem_post(data->param->gard_must);
			data->stop = 1;
		}
		sem_post(data->param->gard_n_eat);
	}
	return ;
}

void	*controling(void *d)
{
	t_data	*data;
	int		alive;
	int		died;

	data = (t_data *)d;
	while (data->param->end_of_simulation == 0)
	{
		alive = get_current_time();
		died = alive - data->param->st_time;
		sem_wait(data->param->gard_alive);
		if (data->param->die_time < alive - data->alive_time)
		{
			sem_post(data->param->gard_alive);
			sem_wait(data->param->gard_end);
			data->param->end_of_simulation = -1;
			printf("%d %d died\n", died, data->id);
			exit (1);
		}
		sem_post(data->param->gard_alive);
		must_eat(data);
	}
	return (NULL);
}

static void	check_n_eat(t_data *data, int s)
{
	if (s == 0)
	{
		sem_wait(data->param->gard_n_eat);
		data->n_eat++;
		sem_post(data->param->gard_n_eat);
	}
	return ;
}

void	print_status(char *message, t_data *d, int *s)
{
	int	time;

	sem_wait(d->param->gard_end);
	if (d->param->end_of_simulation != 0)
	{
		sem_post(d->param->gard_end);
		*s = -1;
		return ;
	}
	sem_post(d->param->gard_end);
	time = get_current_time() - d->param->st_time;
	printf("%d %d %s\n", time, d->id, message);
	return ;
}

void	start(t_data *data)
{
	int	s;

	s = 0;
	if (pthread_create(&data->control, NULL, controling, data) != 0)
		exit (1);
	pthread_detach(data->control);
	while (1 && s == 0)
	{
		print_status("is thinking", data, &s);
		sem_wait(data->param->sem);
		print_status("has taken a fork", data, &s);
		sem_wait(data->param->sem);
		print_status("has taken a fork", data, &s);
		print_status("is eating", data, &s);
		sem_wait(data->param->gard_alive);
		data->alive_time = get_current_time();
		sem_post(data->param->gard_alive);
		my_usleep(data->param->eat_time, data);
		sem_post(data->param->sem);
		sem_post(data->param->sem);
		check_n_eat(data, s);
		print_status("is sleeping", data, &s);
		my_usleep(data->param->sleep_time, data);
	}
	exit (1);
}
