/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasezin <rrasezin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 12:25:31 by rrasezin          #+#    #+#             */
/*   Updated: 2023/06/03 18:02:33 by rrasezin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int	main(int ac, char **av)
{
	sem_t *t;
	t = sem_open("/kkkkkkk", 0);
	if (t == SEM_FAILED)
		printf ("failed\n");
	return (0);
}