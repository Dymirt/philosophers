/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:08:45 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/21 23:16:01 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		threds_work(t_table *table, t_philo **philosophers);
void	simulation_check(t_table *table, t_philo **philosophers);

void	*thread_function(void *arg)
{
	t_philo			*philo;
	int				i;

	philo = (t_philo *)arg;
	philo->last_meal = get_timestamp(philo->tv_start);
	i = 0;
	while (i < philo->table->eat_count && !simulation_is_end(philo)
		&& !philo_is_dead(philo))
	{
		eat_routine(philo);
		if (philo_is_dead(philo) || simulation_is_end(philo))
			break ;
		mutex_printf(philo, "is sleeping");
		usleep(philo->table->time_to_sleep * 1000);
		if (philo_is_dead(philo) || simulation_is_end(philo))
			break ;
		mutex_printf(philo, "is thinking");
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	**philosophers;
	t_table	table;
	int		i;

	philosophers = NULL;
	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (!table_init(&table, argc, argv))
		return (1);
	philos_init(&table, &philosophers);
	threds_work(&table, philosophers);
	i = 0;
	while (i < table.philosophers_count)
	{
		free(philosophers[i]);
		i++;
	}
	return (0);
}

void	free_philo(t_philo **philosophers)
{
	int	i;

	i = 0;
	while (philosophers[i])
	{
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
}

int	threds_work(t_table *table, t_philo **philosophers)
{
	int	i;

	i = 0;
	while (i < table->philosophers_count)
	{
		if (pthread_create(&philosophers[i]->thread, NULL,
				thread_function, philosophers[i]) != 0)
		{
			perror("Failed to create thread");
			return (0);
		}
		i++;
	}
	simulation_check(table, philosophers);
	i = 0;
	while (i < table->philosophers_count)
	{
		if (pthread_join(philosophers[i]->thread, NULL) != 0)
		{
			perror("Failed to join thread");
			return (0);
		}
		i++;
	}
	return (1);
}

void	simulation_check(t_table *table, t_philo **philosophers)
{
	int	i;

	while (!table->simulation_end)
	{
		i = 0;
		while (i < table->philosophers_count)
		{
			if (get_timestamp(philosophers[i]->tv_start)
				- mutex_get_value(table, &philosophers[i]->last_meal)
				> table->time_to_die)
			{
				mutex_printf(philosophers[i], "died");
				mutex_set_value(table, &philosophers[i]->is_dead, 1);
				mutex_set_value(table, &table->simulation_end, 1);
				break ;
			}
			i++;
		}
	}
}
