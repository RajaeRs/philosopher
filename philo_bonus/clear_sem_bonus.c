/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_sem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 00:52:46 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/13 00:57:51 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_sem(void)
{
	sem_unlink("/must_eat");
	sem_unlink("/end");
	sem_unlink("/alive");
	sem_unlink("/n_eat");
	sem_unlink("/forks");
	sem_unlink("/sleep");
}

int	close_and_free(t_param *param, int i)
{
	if (i > 1)
		sem_close(param->gard_end);
	if (i > 2)
		sem_close(param->gard_alive);
	if (i > 3)
		sem_close(param->gard_n_eat);
	if (i > 4)
		sem_close(param->sem);
	if (i > 5)
		sem_close(param->gard_must);
	sem_unlink("/must_eat");
	sem_unlink("/end");
	sem_unlink("/alive");
	sem_unlink("/n_eat");
	sem_unlink("/forks");
	free (param);
	return (1);
}
