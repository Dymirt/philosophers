/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:47:51 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/26 14:47:01 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_init(t_table *table);
int	mutexes_init(t_table *table);
int	final_cheks(t_table *table);
int	set_eat_count(t_table *table, int argc, char **argv);

int	table_init(t_table *table, int argc, char **argv)
{
	table->philosophers_count = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (!final_cheks(table))
		return (0);
	table->simulation_end = 0;
	table->forks = malloc(table->philosophers_count * sizeof(pthread_mutex_t));
	if (table->forks == NULL)
	{
		printf("Error: malloc failed\n");
		return (0);
	}
	if (!fork_init(table))
		return (0);
	if (!set_eat_count(table, argc, argv))
		return (0);
	mutexes_init(table);
	return (1);
}

int	fork_init(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philosophers_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			perror("Failed to initialize mutex");
			return (0);
		}
		i++;
	}
	return (1);
}

int	mutexes_init(t_table *table)
{
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
	{
		perror("Failed to initialize mutex");
		return (0);
	}
	if (pthread_mutex_init(&table->change_value_mutex, NULL) != 0)
	{
		perror("Failed to initialize mutex");
		return (0);
	}
	return (1);
}

int	final_cheks(t_table *table)
{
	if (table->time_to_die <= 0
		|| table->time_to_eat <= 0
		|| table->time_to_sleep <= 0
		|| table->philosophers_count <= 0)
	{
		printf("Error: wrong number of arguments\n");
		return (0);
	}
	return (1);
}

int	set_eat_count(t_table *table, int argc, char **argv)
{
	table->eat_count = -1;
	if (argc == 6)
	{
		table->eat_count = ft_atoi(argv[5]);
		if (table->eat_count <= 0)
		{
			printf("Error: wrong number of arguments\n");
			return (0);
		}
	}
	return (1);
}
