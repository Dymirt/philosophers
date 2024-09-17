/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:40:06 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/18 00:43:39 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void eat_routine(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%d %d has taken a fork\n", get_timestamp(philo->tv_start), philo->id);
		if (philo->table->philosophers_count == 1)
		{
			usleep(philo->table->time_to_die * 1000);
			return ;
		}
		pthread_mutex_lock(philo->right_fork);
		printf("%d %d has taken a fork\n", get_timestamp(philo->tv_start), philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		printf("%d %d has taken a fork\n", get_timestamp(philo->tv_start), philo->id);
		if (philo->table->philosophers_count == 1)
		{
			usleep(philo->table->time_to_die * 1000);
			return ;
		}
		pthread_mutex_lock(philo->left_fork);
		printf("%d %d has taken a fork\n", get_timestamp(philo->tv_start), philo->id);
	}
	philo->eat_count++;
	printf("%d %d is eating\n", get_timestamp(philo->tv_start), philo->id);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
