/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_wrapers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 23:23:07 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/20 23:35:39 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_printf(t_philo *philo, char *str)
{
	if (!philo_is_dead(philo) && !simulation_is_end(philo))
	{
		pthread_mutex_lock(&philo->table->print_mutex);
		printf("%d %d %s\n", get_timestamp(philo->tv_start), philo->id, str);
		pthread_mutex_unlock(&philo->table->print_mutex);
	}
}

// SETTERS
void	mutex_set_value(t_table *table, int *key, int value)
{
	pthread_mutex_lock(&table->change_value_mutex);
	*key = value;
	pthread_mutex_unlock(&table->change_value_mutex);
}

// GETTERS

int	mutex_get_value(t_table *table, int *key)
{
	int	value;

	pthread_mutex_lock(&table->change_value_mutex);
	value = *key;
	pthread_mutex_unlock(&table->change_value_mutex);
	return (value);
}

int	philo_is_dead(t_philo *philo)
{
	return (mutex_get_value(philo->table, &philo->is_dead));
}

int	simulation_is_end(t_philo *philo)
{
	return (mutex_get_value(philo->table, &philo->table->simulation_end));
}
