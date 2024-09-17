/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:10:14 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/17 21:44:26 by dkolida          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct table_s
{
	int	philosophers_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	eat_count;
}	t_table;

typedef struct philo_s
{
	int				id;
	pthread_t		thread;
	struct timeval	*tv_start;
	t_table			*table;
	int				eat_count;
}	t_philo;

int	get_timestamp(struct timeval *tv_start);

#endif
