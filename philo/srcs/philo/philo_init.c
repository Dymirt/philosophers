/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:47:58 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/21 23:17:00 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_init(t_table *table, t_philo **philosophers, int i);

int	philos_init(t_table *table, t_philo ***philosophers)
{
	int				i;
	struct timeval	*shared_tv_start;

	i = 0;
	shared_tv_start = malloc(sizeof(struct timeval));
	if (shared_tv_start == NULL)
	{
		printf("Error: malloc failed\n");
		return (0);
	}
	*philosophers = malloc(sizeof(t_philo *) * table->philosophers_count);
	if (*philosophers == NULL)
	{
		printf("Error: malloc failed\n");
		return (0);
	}
	while (i < table->philosophers_count)
	{
		if (!philo_init(table, *philosophers, i))
			return (0);
		(*philosophers)[i]->tv_start = shared_tv_start;
		i++;
	}
	gettimeofday(shared_tv_start, NULL);
	return (1);
}

int	philo_init(t_table *table, t_philo **philosophers, int i)
{
	philosophers[i] = malloc(sizeof(t_philo));
	if (philosophers[i] == NULL)
	{
		printf("Error: malloc failed\n");
		return (0);
	}
	philosophers[i]->id = i + 1;
	philosophers[i]->table = table;
	philosophers[i]->eat_count = 0;
	philosophers[i]->left_fork = &table->forks[i];
	philosophers[i]->right_fork
		= &table->forks[(i + 1) % table->philosophers_count];
	philosophers[i]->is_dead = 0;
	return (1);
}
