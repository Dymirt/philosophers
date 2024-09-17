/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:08:45 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/17 21:52:13 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_init(t_table *table, t_philo ***philosophers);

void	*thread_function(void *arg)
{
	t_philo			*philo;
	struct timeval	tv_current;

	philo = (t_philo *)arg;
	gettimeofday(&tv_current, NULL);
	printf("%d %d is eating\n", get_timestamp(philo->tv_start), philo->id);
	usleep(philo->table->time_to_eat * 1000);
	printf("%d %d is sleeping\n", get_timestamp(philo->tv_start), philo->id);
	usleep(philo->table->time_to_sleep * 1000);
	printf("%d %d is thinking\n", get_timestamp(philo->tv_start), philo->id);
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
	table.eat_count = 0;
	if (argc == 6)
		table.eat_count = atoi(argv[5]);
	philo_init(&table, &philosophers);
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

int philo_init(t_table *table, t_philo ***philosophers)
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
		(*philosophers)[i] = malloc(sizeof(t_philo));
		if ((*philosophers)[i] == NULL)
		{
			printf("Error: malloc failed\n");
			return (0);
		}
		(*philosophers)[i]->id = i + 1;
		(*philosophers)[i]->tv_start = shared_tv_start;
		(*philosophers)[i]->table = table;
		i++;
	}
	gettimeofday(shared_tv_start, NULL);
	return (1);
}
