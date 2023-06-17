/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controling.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:40:44 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/17 16:21:29 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	n_philo_eating(t_data *data)
{
	int		i;
	int		philo;

	i = 0;
	philo = 0;
	while (i < data->param->n_philo)
	{
		if (data[i].philo_must_eat == 1)
			philo++;
		i++;
	}
	return (philo);
}

int	end(t_data *data)
{
	int		i;

	i = 0;
	pthread_mutex_lock(&data[i].param->gard_end);
	if (data[i].param->end_of_simulation == -1)
	{
		pthread_mutex_unlock(&data[i].param->gard_end);
		return (-1);
	}
	pthread_mutex_unlock(&data[i].param->gard_end);
	return (0);
}

int	number_of_eat(t_data *data)
{
	int		i;

	i = 0;
	if (end(data) != 0)
		return (-1);
	while (i < data->param->n_philo)
	{
		pthread_mutex_lock(&data->param->gard_n_eat);
		if (data[i].n_eat >= data[i].param->must_eat
			&& data[i].philo_must_eat == 0)
			data[i].philo_must_eat = 1;
		pthread_mutex_unlock(&data->param->gard_n_eat);
		i++;
	}
	i = 0;
	if (n_philo_eating(data) == data->param->n_philo)
	{
		pthread_mutex_lock(&data[i].param->gard_end);
		data->param->end_of_simulation = -1;
		pthread_mutex_unlock(&data[i].param->gard_end);
		return (-1);
	}
	return (0);
}

int	control_death(t_data	*data)
{
	int		i;

	i = 0;
	if (end(data) != 0)
		return (-1);
	while (i < data->param->n_philo)
	{
		pthread_mutex_lock(&data->param->gard_alive);
		if (data[i].param->die_time <= get_current_time() - data[i].alive_time)
		{
			pthread_mutex_unlock(&data->param->gard_alive);
			pthread_mutex_lock(&data[i].param->gard_end);
			data[i].param->end_of_simulation = -1;
			pthread_mutex_unlock(&data[i].param->gard_end);
			usleep(100);
			printf("%d %d died\n", \
				(get_current_time() - data[i].param->st_time), data[i].id);
			return (-1);
		}
		pthread_mutex_unlock(&data->param->gard_alive);
		i++;
	}
	return (0);
}

void	control_simulation(t_data *data, t_param *param)
{
	int	p;

	p = 0;
	while (p == 0)
	{
		p = control_death(data);
		if (param->must_eat != -1)
		{
			p = number_of_eat(data);
			if (p != 0)
				break ;
		}
	}
	return ;
}
