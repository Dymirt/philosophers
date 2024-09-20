/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:40:06 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/21 00:13:31 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		get_forks(t_philo *philo);
void	mutex_fork_lock(t_philo *philo, pthread_mutex_t *fork);

void	eat_routine(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	if (philo_is_dead(philo) || simulation_is_end(philo))
		return ;
	if (!get_forks(philo))
	{
		usleep(philo->table->time_to_die * 1000);
		return ;
	}
	mutex_printf(philo, "is eating");
	mutex_set_value(table, &philo->last_meal, get_timestamp(philo->tv_start));
	usleep(philo->table->time_to_eat * 1000);
	mutex_set_value(table, &philo->last_meal, get_timestamp(philo->tv_start));
	philo->eat_count++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

int	get_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	mutex_fork_lock(philo, first_fork);
	mutex_printf(philo, "has taken a fork");
	if (philo->table->philosophers_count == 1)
		return (0);
	mutex_fork_lock(philo, second_fork);
	mutex_printf(philo, "has taken a fork");
	return (1);
}

void	mutex_fork_lock(t_philo *philo, pthread_mutex_t *fork)
{
	if (philo->table->philosophers_count != 1
		&& !philo_is_dead(philo)
		&& !simulation_is_end(philo))
		pthread_mutex_lock(fork);
}
