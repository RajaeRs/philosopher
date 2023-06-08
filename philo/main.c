/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:14:21 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/01 12:14:28 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_current_time(void)
{
	struct timeval	tv;
	struct timezone	tz;
	int				time;

	time = 0;
	if (gettimeofday(&tv, &tz) != 0)
		return (-1);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	my_usleep(int time_to_sleep, t_data *data)
{
	int	start_time;

	start_time = get_current_time();
	while (1)
	{
		if (time_to_sleep <= (get_current_time() - start_time))
			break ;
		pthread_mutex_lock(&data->param->gard_end);
		if (data->param->end_of_simulation != 0)
		{
			pthread_mutex_unlock(&data->param->gard_end);
			break ;
		}
		pthread_mutex_unlock(&data->param->gard_end);
		usleep(80);
	}
}

void	finish_simulation(t_data *data, t_param *param)
{
	int	i;

	i = 0;
	while (i < param->n_philo)
	{
		pthread_join(data[i].philo, NULL);
		i++;
	}
	pthread_mutex_destroy(&param->gard_end);
	pthread_mutex_destroy(&param->gard_n_eat);
	pthread_mutex_destroy(&param->gard_alive);
	i = 0;
	while (i < param->n_philo)
	{
		pthread_mutex_destroy(&param->mutex[i]);
		i++;
	}
	free(param->mutex);
	free(param);
	free(data);
}

int	main(int ac, char **av)
{
	t_data		*data;
	t_param		*param;
	int			i;

	i = 1;
	if (ac < 5 || ac > 6)
		return (print_help());
	while (i < ac)
	{
		if (check_error(av[i]) == 1)
			return (print_help());
		i++;
	}
	if (ft_atoi(av[1]) == 0)
		return (0);
	i = 0;
	param = get_philo_param(ac, av);
	data = ft_calloc(param->n_philo, sizeof(t_data));
	start_simulation(data, param);
	control_simulation(data, param);
	finish_simulation(data, param);
	return (0);
}
