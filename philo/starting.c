/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:43:45 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/01 12:15:26 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(char *message, t_data *d, int *s)
{
	int	time;

	pthread_mutex_lock(&d->param->gard_end);
	if (d->param->end_of_simulation != 0)
	{
		*s = -1;
		pthread_mutex_unlock(&d->param->gard_end);
		return ;
	}
	pthread_mutex_unlock(&d->param->gard_end);
	time = get_current_time() - d->param->st_time;
	printf("%d %d %s\n", time, d->id, message);
	return ;
}

int	taking_forks(t_data *d, int *s)
{
	print_status("is thinking", d, s);
	pthread_mutex_lock(&d->param->mutex[d->id - 1]);
	print_status("has taken a fork", d, s);
	if (d->param->n_philo == 1)
	{
		pthread_mutex_unlock(&d->param->mutex[d->id - 1]);
		my_usleep(d->param->die_time, d);
		return (-1);
	}
	if (d->id == d->param->n_philo)
		pthread_mutex_lock(&d->param->mutex[0]);
	else
		pthread_mutex_lock(&d->param->mutex[d->id]);
	print_status("has taken a fork", d, s);
	return (0);
}

void	add_n_eat(t_data *data, int s)
{
	if (s == 0)
	{
		pthread_mutex_lock(&data->param->gard_n_eat);
		data->n_eat++;
		pthread_mutex_unlock(&data->param->gard_n_eat);
	}
}

void	*start(void *data)
{
	t_data	*d;
	int		s;

	d = (t_data *)data;
	s = 0;
	while (1 && s == 0)
	{
		if (taking_forks(d, &s) != 0)
			return (NULL);
		print_status("is eating", d, &s);
		add_n_eat(d, s);
		pthread_mutex_lock(&d->param->gard_alive);
		d->alive_time = get_current_time();
		pthread_mutex_unlock(&d->param->gard_alive);
		my_usleep(d->param->eat_time, d);
		pthread_mutex_unlock(&d->param->mutex[d->id - 1]);
		if (d->id == d->param->n_philo)
			pthread_mutex_unlock(&d->param->mutex[0]);
		else
			pthread_mutex_unlock(&d->param->mutex[d->id]);
		print_status("is sleeping", d, &s);
		my_usleep(d->param->sleep_time, d);
	}
	return (NULL);
}

void	start_simulation(t_data *data, t_param *param)
{
	int	i;

	i = 0;
	while (i < param->n_philo)
	{
		if (pthread_mutex_init(&param->mutex[i], NULL) != 0)
			write(2, "error\n", 6);
		i++;
	}
	i = 0;
	while (i < param->n_philo)
	{
		data[i].id = i + 1;
		data[i].param = param;
		data[i].n_eat = 0;
		data[i].philo_must_eat = 0;
		data[i].alive_time = get_current_time();
		if (pthread_create(&data[i].philo, NULL, start, &data[i]) != 0)
			write(2, "error\n", 6);
		i++;
		usleep(100);
	}
	return ;
}
