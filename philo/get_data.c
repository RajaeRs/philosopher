/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 22:53:31 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/01 12:08:22 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_param	*get_philo_param(int ac, char **av)
{
	t_param	*param;

	param = ft_calloc(sizeof(t_param), 1);
	pthread_mutex_init(&param->gard_end, NULL);
	pthread_mutex_init(&param->gard_alive, NULL);
	pthread_mutex_init(&param->gard_n_eat, NULL);
	param->st_time = get_current_time();
	param->n_philo = ft_atoi(av[1]);
	param->die_time = ft_atoi(av[2]);
	param->eat_time = ft_atoi(av[3]);
	param->sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		param->must_eat = ft_atoi(av[5]);
	else
		param->must_eat = -1;
	param->mutex = ft_calloc(param->n_philo, sizeof(pthread_mutex_t));
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
