/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:10:14 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/21 00:13:59 by dkolida          ###   ########.fr       */
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
	int				philosophers_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count;
	pthread_mutex_t	*forks;
	int				simulation_end;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	change_value_mutex;
}	t_table;

typedef struct philo_s
{
	int				id;
	pthread_t		thread;
	struct timeval	*tv_start;
	t_table			*table;
	int				eat_count;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				last_meal;
	int				is_dead;
}	t_philo;

// routines
void	eat_routine(t_philo *philo);

// helpers
int		get_timestamp(struct timeval *tv_start);

// mutex_wrapers
int		simulation_is_end(t_philo *philo);
int		philo_is_dead(t_philo *philo);
void	mutex_printf(t_philo *philo, char *str);
void	mutex_set_value(t_table *table, int *key, int value);
int		mutex_get_value(t_table *table, int *key);

#endif
