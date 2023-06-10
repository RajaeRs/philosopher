/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 12:25:31 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/10 22:06:33 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
			// if (data->param->must_eat != -1 && data->stop == 0)
			// 	sem_post(data->param->gard_must);
			data->param->end_of_simulation = -1;
			printf("%d %d died\n", died, data->id);
			exit (1);
		}
		sem_post(data->param->gard_alive);
		if (data->param->must_eat != -1 && data->stop == 0)
		{
			sem_wait(data->param->gard_n_eat);
			if (data->n_eat  >= data->param->must_eat)
			{
				sem_post(data->param->gard_must);
				data->stop = 1;
			}
			sem_post(data->param->gard_n_eat);
		}
	}
	return (NULL);
}

void	start(t_data *data)
{
	int	s;

	s = 0;
	pthread_create(&data->control, NULL, controling, data);
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
		if (s == 0)
		{
			sem_wait(data->param->gard_n_eat);
			data->n_eat++;
			sem_post(data->param->gard_n_eat);
		}
		print_status("is sleeping", data, &s);
		my_usleep(data->param->sleep_time, data);
	}
	exit (1);
}


void	*waiting(void *d)
{
	t_data	*data;
	int		i;

	data = (t_data *)d;
	i = 0;
	my_usleep(data->param->eat_time, data);
	while (i < data->param->n_philo)
	{
		sem_wait(data->param->gard_must);
		i++;
	}
	kill(data[0].philo, SIGINT);
	return (NULL);
}


int	mmain(int ac, char **av)
{
	t_param	*param;
	t_data	*data;
	pthread_t	wait_eat;
	int		i;

	i = 1;
	if (ac < 5 || ac > 6)
		return (1);
	while (i < ac)
	{
		if (check_error(av[i]) == 1)
			return (print_help());
		i++;
	}
	i = 0;
	param = get_philo_param(ac, av);
	if (param == NULL)
	{
		write(2, "sem_open failed\n", 16);
		return (1);
	}
	data = ft_calloc(sizeof(t_data), param->n_philo);
	data->param = param;
	while (i < param->n_philo)
	{
		data[i].id = i + 1;
		data[i].philo = fork();
		if (data[i].philo == 0)
		{
			data[i].param = param;
			data[i].n_eat = 0;
			data[i].alive_time = get_current_time();
			data[i].stop = 0;
			if (param->must_eat != -1)
				sem_wait(data[i].param->gard_must);
			start(&data[i]);
		}
		i++;
	}
	i = 0;
	if (param->must_eat != -1)
	{
		pthread_create(&wait_eat, NULL, waiting, data);
		pthread_detach(wait_eat);
	}
	waitpid(-1, NULL, 0);
	while (i < param->n_philo)
	{
		if (param->must_eat != -1)
			sem_post(param->gard_must);
		kill(data[i].philo, SIGINT);
		i++;
	}
	// if (param->must_eat != -1)
	// {
	// 	pthread_join(wait_eat, NULL);
	// }
	sem_close(param->gard_alive);
	sem_close(param->gard_n_eat);
	sem_close(param->sem);
	if (param->must_eat != -1)
		sem_close(param->gard_must);
	sem_post(param->gard_end);
	sem_close(param->gard_end);
	free (param);
	free (data);
	return (0);
}

int main(int ac, char **av)
{
	mmain(ac, av);
	
}