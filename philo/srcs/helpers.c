/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolida <dkolida@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:15:25 by dkolida           #+#    #+#             */
/*   Updated: 2024/09/17 21:47:00 by dkolida          ###   ########.fr       */
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
