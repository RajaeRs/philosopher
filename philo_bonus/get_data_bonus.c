/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:58:03 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/13 00:58:05 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	must_eat_sem(t_param *param)
{
	sem_unlink("/must_eat");
	param->gard_must = sem_open("/must_eat", O_CREAT \
		| O_EXCL, 0644, param->n_philo);
	if (param->gard_must == SEM_FAILED)
		return (close_and_free(param, 5));
	sem_unlink("/sleep");
	param->gard_sleep = sem_open("/sleep", O_CREAT | O_EXCL, 0644, 1);
	if (param->gard_sleep == SEM_FAILED)
		return (close_and_free(param, 6));
	return (0);
}

int	create_semaphor(t_param *param, int ac)
{
	sem_unlink("/end");
	param->gard_end = sem_open("/end", O_CREAT | O_EXCL, 0644, 1);
	if (param->gard_end == SEM_FAILED)
		return (close_and_free(param, 1));
	sem_unlink("/alive");
	param->gard_alive = sem_open("/alive", O_CREAT | O_EXCL, 0644, 1);
	if (param->gard_alive == SEM_FAILED)
		return (close_and_free(param, 2));
	sem_unlink("/n_eat");
	param->gard_n_eat = sem_open("/n_eat", O_CREAT | O_EXCL, 0644, 1);
	if (param->gard_n_eat == SEM_FAILED)
		return (close_and_free(param, 3));
	sem_unlink("/forks");
	param->sem = sem_open("/forks", O_CREAT | O_EXCL, 0644, param->n_philo);
	if (param->sem == SEM_FAILED)
		return (close_and_free(param, 4));
	if (ac == 6)
		return (must_eat_sem(param));
	return (0);
}

t_param	*get_philo_param(int ac, char **av)
{
	t_param	*param;

	param = ft_calloc(sizeof(t_param), 1);
	param->st_time = get_current_time();
	param->n_philo = ft_atoi(av[1]);
	param->die_time = ft_atoi(av[2]);
	param->eat_time = ft_atoi(av[3]);
	param->sleep_time = ft_atoi(av[4]);
	param->end_of_simulation = 0;
	param->stop_sleeping = 0;
	if (ac == 6)
		param->must_eat = ft_atoi(av[5]);
	else
		param->must_eat = -1;
	if (create_semaphor(param, ac) != 0)
		return (NULL);
	return (param);
}

int	check_valide_data(char *data)
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
