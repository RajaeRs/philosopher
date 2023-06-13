/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_control_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:37:32 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/12 23:59:44 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	get_current_time(void)
{
	struct timeval	tv;
	int				time;

	time = 0;
	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	my_usleep(int time_to_sleep, t_data *data)
{
	int	start_time;

	(void)data;
	start_time = get_current_time();
	while (1)
	{
		if (time_to_sleep <= (get_current_time() - start_time))
			break ;
		sem_wait(data->param->gard_end);
		if (data->param->end_of_simulation != 0)
		{
			sem_post(data->param->gard_end);
			break ;
		}
		sem_post(data->param->gard_end);
		usleep(80);
	}
	return ;
}

void	my_usleep_2(int time_to_sleep, t_data *data)
{
	int	start_time;

	(void)data;
	start_time = get_current_time();
	while (1)
	{
		if (time_to_sleep <= (get_current_time() - start_time))
			break ;
		sem_wait(data->param->gard_sleep);
		if (data->param->stop_sleeping != 0)
		{
			sem_post(data->param->gard_sleep);
			break ;
		}
		sem_post(data->param->gard_sleep);
		usleep(80);
	}
	return ;
}
