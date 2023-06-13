/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 12:25:31 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/13 00:52:21 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*waiting(void *d)
{
	t_data	*data;
	int		i;

	data = (t_data *)d;
	i = 0;
	my_usleep_2(data->param->eat_time, data);
	while (i < data->param->n_philo)
	{
		sem_wait(data->param->gard_must);
		i++;
	}
	kill(data[0].philo, SIGINT);
	return (NULL);
}

void	start_simulation(t_data *data, t_param *param)
{
	int	i;

	i = 0;
	while (i < param->n_philo)
	{
		data[i].id = i + 1;
		data[i].param = param;
		data[i].n_eat = 0;
		data[i].alive_time = get_current_time();
		data[i].stop = 0;
		data[i].philo = fork();
		if (data[i].philo == 0)
		{
			if (param->must_eat != -1)
				sem_wait(data[i].param->gard_must);
			start(&data[i]);
		}
		else
			i++;
	}
	return ;
}

void	finish_simulation(t_data *data,
	t_param *param, int i, pthread_t wait_eat)
{
	sem_wait(data->param->gard_sleep);
	param->stop_sleeping = -1;
	sem_post(data->param->gard_sleep);
	usleep(50);
	while (i < param->n_philo)
	{
		if (param->must_eat != -1)
			sem_post(param->gard_must);
		kill(data[i].philo, SIGINT);
		i++;
	}
	if (param->must_eat != -1)
		pthread_join(wait_eat, NULL);
	sem_close(param->gard_alive);
	sem_close(param->gard_n_eat);
	sem_close(param->sem);
	if (param->must_eat != -1)
	{
		sem_close(param->gard_must);
		sem_close(param->gard_sleep);
	}
	sem_post(param->gard_end);
	sem_close(param->gard_end);
	return ;
}

int	check_error(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac < 5 || ac > 6)
	{
		if (ac < 5)
			write (2, "[!] Not enough arguments to start\n", 34);
		else
			write (2, "[!] to more arguments\n", 22);
		return (1);
	}
	while (i < ac)
	{
		if (check_valide_data(av[i]) == 1)
			return (print_help());
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_param		*param;
	t_data		*data;
	pthread_t	wait_eat;

	if (check_error(ac, av) != 0)
		return (1);
	param = get_philo_param(ac, av);
	if (param == NULL)
	{
		write(2, "sem_open failed\n", 16);
		return (1);
	}
	data = ft_calloc(sizeof(t_data), param->n_philo);
	start_simulation(data, param);
	if (param->must_eat != -1)
		pthread_create(&wait_eat, NULL, waiting, data);
	else
		wait_eat = (long unsigned int)NULL;
	waitpid(-1, NULL, 0);
	finish_simulation(data, param, 0, wait_eat);
	unlink_sem();
	free (param);
	free (data);
	return (0);
}
