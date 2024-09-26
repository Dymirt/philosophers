/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:08:45 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/26 13:43:58 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		threds_work(t_table *table, t_philo **philosophers);
void	simulation_check(t_table *table, t_philo **philosophers);
void	free_at_end(t_table *table, t_philo **philosophers);

void	*thread_function(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	mutex_set_value(philo->table, &philo->last_meal,
		get_timestamp(philo->tv_start));
	while (!simulation_is_end(philo) && !philo_is_dead(philo))
	{
		if (philo_is_dead(philo) || simulation_is_end(philo))
			break ;
		eat_routine(philo);
		if (philo_is_dead(philo) || simulation_is_end(philo))
			break ;
		mutex_printf(philo, "is sleeping");
		usleep(philo->table->time_to_sleep * 1000);
		if (philo_is_dead(philo) || simulation_is_end(philo))
			break ;
		mutex_printf(philo, "is thinking");
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	**philosophers;
	t_table	table;

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
	free_at_end(&table, philosophers);
	return (0);
}

void	free_at_end(t_table *table, t_philo **philosophers)
{
	int	i;

	i = 0;
	while (i < table->philosophers_count)
	{
		if (philosophers[i]->tv_start != NULL)
		{
			free(philosophers[i]->tv_start);
			philosophers[i]->tv_start = NULL;
		}
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
	i = 0;
	while (i < table->philosophers_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->change_value_mutex);
	free(table->forks);
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

void	simulation_check(t_table *table, t_philo **philos)
{
	int	i;
	int	full_philos;

	full_philos = 0;
	while (!table->simulation_end)
	{
		if (full_philos == table->philosophers_count)
			mutex_set_value(table, &table->simulation_end, 1);
		i = 0;
		while (i < table->philosophers_count && !table->simulation_end)
		{
			if (is_full(table, philos, i) && !philo_is_dead(philos[i]))
			{
				mutex_set_value(table, &philos[i]->is_dead, 1);
				full_philos++;
			}
			if (time_to_die(table, philos, i) && !philo_is_dead(philos[i]))
			{
				mutex_printf(philos[i], "died");
				mutex_set_value(table, &philos[i]->is_dead, 1);
				mutex_set_value(table, &table->simulation_end, 1);
			}
			i++;
		}
	}
}
