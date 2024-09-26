/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:15:25 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/26 13:27:33 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_timestamp(struct timeval *tv_start)
{
	struct timeval	tv_current;

	gettimeofday(&tv_current, NULL);
	return ((tv_current.tv_sec - tv_start->tv_sec) * 1000
		+ (tv_current.tv_usec - tv_start->tv_usec) / 1000);
}

int	time_to_die(t_table *table, t_philo **philos, int i)
{
	return (get_timestamp(philos[i]->tv_start)
		- mutex_get_value(table, &philos[i]->last_meal)
		> table->time_to_die - 1);
}

int	is_full(t_table *table, t_philo **philos, int i)
{
	return (mutex_get_value(table, &philos[i]->eat_count) == table->eat_count);
}
