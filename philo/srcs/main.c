/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:08:45 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/21 00:24:43 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philos_init(t_table *table, t_philo ***philosophers);
int	philo_init(t_table *table, t_philo **philosophers, int i, struct timeval *shared_tv_start);

void	*thread_function(void *arg)
{
	t_philo			*philo;
	int				i;

	philo = (t_philo *)arg;
	philo->last_meal = get_timestamp(philo->tv_start);
	i = 0;
	while (i < philo->table->eat_count && !simulation_is_end(philo) && !philo_is_dead(philo))
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
	table.philosophers_count = atoi(argv[1]);
	table.time_to_die = atoi(argv[2]);
	table.time_to_eat = atoi(argv[3]);
	table.time_to_sleep = atoi(argv[4]);
	table.forks = malloc(table.philosophers_count * sizeof(pthread_mutex_t));
	table.simulation_end = 0;
	if (table.forks == NULL)
	{
		printf("Error: malloc failed\n");
		return (1);
	}
	table.eat_count = 0;
	if (argc == 6)
		table.eat_count = atoi(argv[5]);
	i = 0;
	while (i < table.philosophers_count)
	{
		if (pthread_mutex_init(&table.forks[i], NULL) != 0)
		{
			perror("Failed to initialize mutex");
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&table.print_mutex, NULL) != 0)
	{
		perror("Failed to initialize mutex");
		return (1);
	}
	if (pthread_mutex_init(&table.change_value_mutex, NULL) != 0)
	{
		perror("Failed to initialize mutex");
		return (1);
	}
	philos_init(&table, &philosophers);
	i = 0;
	while (i < table.philosophers_count)
	{
		if (pthread_create(&philosophers[i]->thread, NULL, thread_function, philosophers[i]) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	while (!table.simulation_end)
	{
		i = 0;
		while (i < table.philosophers_count)
		{
			if (get_timestamp(philosophers[i]->tv_start) - mutex_get_value(&table, &philosophers[i]->last_meal) > table.time_to_die)
			{
				mutex_printf(philosophers[i], "died");
				mutex_set_value(&table, &philosophers[i]->is_dead, 1);
				mutex_set_value(&table, &table.simulation_end, 1);
				break ;
			}
			i++;
		}
	}
	i = 0;
	while (i < table.philosophers_count)
	{
		if (pthread_join(philosophers[i]->thread, NULL) != 0)
		{
			perror("Failed to join thread");
			return (1);
		}
		i++;
	}
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
		if (!philo_init(table, *philosophers, i, shared_tv_start))
		{
			free_philo(*philosophers);
			return (0);
		}
		i++;
	}
	gettimeofday(shared_tv_start, NULL);
	return (1);
}

int philo_init(t_table *table, t_philo **philosophers, int i, struct timeval *shared_tv_start)
{
	philosophers[i] = malloc(sizeof(t_philo));
	if (philosophers[i] == NULL)
	{
		printf("Error: malloc failed\n");
		return (0);
	}
	philosophers[i]->id = i + 1;
	philosophers[i]->tv_start = shared_tv_start;
	philosophers[i]->table = table;
	philosophers[i]->eat_count = 0;
	philosophers[i]->left_fork = &table->forks[i];
	philosophers[i]->right_fork = &table->forks[(i + 1) % table->philosophers_count];
	philosophers[i]->is_dead = 0;
	return (1);
}
