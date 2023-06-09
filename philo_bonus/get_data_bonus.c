/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:58:03 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/09 15:43:38 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_param	*get_philo_param(int ac, char **av)
{
	t_param	*param;

	param = ft_calloc(sizeof(t_param), 1);
	param->st_time = get_current_time();
	param->n_philo = ft_atoi(av[1]);
	param->die_time = ft_atoi(av[2]);
	param->eat_time = ft_atoi(av[3]);
	param->sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		param->must_eat = ft_atoi(av[5]);
	else
		param->must_eat = -1;
	param->gard_end = sem_open("/end", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/end");
	param->gard_alive = sem_open("/alive", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/alive");
	param->gard_n_eat = sem_open("/n_eat", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/n_eat");
	param->sem = sem_open("/forks", O_CREAT | O_EXCL, 0644, param->n_philo);
	sem_unlink("/forks");
	if (param->sem == SEM_FAILED || param->gard_end == SEM_FAILED
		|| param->gard_alive == SEM_FAILED || param->gard_n_eat == SEM_FAILED)
	{
		free (param);
		return (NULL);
	}
	if (ac == 6)
	{
		param->gard_must = sem_open("/must_eat", O_CREAT | O_EXCL, 0644, param->n_philo);
		sem_unlink("/must_eat");
		if (param->gard_must == SEM_FAILED)
		{
			free (param);
			return (NULL);
		}
	}
	param->end_of_simulation = 0;
	return (param);
}

int	check_error(char *data)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	if (data[i] == '+' || data[i] == '-')
	{
		if (data[i++] == '-')
			return (1);
		if (data[i] == '\0')
			return (1);
	}
	while (data[i] >= '0' && data[i] <= '9')
	{
		result = result * 10 + data[i++] - '0';
		if (result > 2147483647)
			return (1);
	}
	if (data[i] != '\0')
		return (1);
	if (result == 0)
		return (1);
	return (0);
}

int	print_help(void)
{
	write(2, "\nAll number most be positive\n\n", 30);
	write(2, "1 : Number_of_philosophers\n", 27);
	write(2, "2 : time_to_die\n", 16);
	write(2, "3 : time_to_eat\n", 16);
	write(2, "4 : time_to_sleep\n", 18);
	write(2, "5 : [number_of_times_each_philosopher_must_eat]\n\n", 49);
	return (1);
}
