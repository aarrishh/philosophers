/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:51:05 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/11 21:17:27 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*for_eat_2(t_philo *ph)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < ph->info->num_philos)
	{
		pthread_mutex_lock(&ph->info->check_eat_count_mutex);
		if (ph[i].eat_count >= ph->info->eat_limit)
			count++;
		pthread_mutex_unlock(&ph->info->check_eat_count_mutex);
		i++;
	}
	if (count == ph->info->num_philos)
	{
		pthread_mutex_lock(&ph->info->alive_mutex);
		if (ph->info->is_alive == 1)
		{
			ph->info->is_alive = -1;
			long long timestamp = current_timestamp_ms() - ph->info->start_time;
			printf("[%lld] DINNER IS OVER\n", timestamp);
		}
		pthread_mutex_unlock(&ph->info->alive_mutex);
		return (NULL);
	}
	return (NULL);
}

void	*for_eat(void* philo)
{
	t_philo	*ph = (t_philo *)philo;

	while (1)
	{
		usleep(1000);
		pthread_mutex_lock(&ph->info->alive_mutex);
		if (ph->info->is_alive == -1)
		{
			pthread_mutex_unlock(&ph->info->alive_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&ph->info->alive_mutex);
		if (for_eat_2(ph) == NULL)
			return (NULL);
	}
}

int	check_alive(t_philo *ph)
{
	pthread_mutex_lock(&ph->info->alive_mutex);
	if (ph->info->is_alive == -1)
	{
		pthread_mutex_unlock(&ph->info->alive_mutex);
		return -1;
	}
	pthread_mutex_unlock(&ph->info->alive_mutex);
	return 0;
}

