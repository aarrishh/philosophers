/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:51:05 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/12 16:27:33 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	for_eat_2(t_philo *ph)
{
	int	i;
	long long timestamp;

	i = -1;
	while (++i < ph->info->num_philos)
	{
		pthread_mutex_lock(&ph->info->check_eat_count_mutex);
		if (ph[i].eat_count >= ph->info->eat_limit)
			(ph->info->done_eating)++;
		pthread_mutex_unlock(&ph->info->check_eat_count_mutex);
	}
	if (ph->info->done_eating == ph->info->num_philos)
	{
		pthread_mutex_lock(&ph->info->alive_mutex);
		if (ph->info->is_alive == 1)
		{
			ph->info->is_alive = -1;
			timestamp = current_timestamp_ms() - ph->info->start_time;
			printf("[%lld] DINNER IS OVER\n", timestamp);
		}
		pthread_mutex_unlock(&ph->info->alive_mutex);
		return (1);
	}
	return (0);
}
// while(i < ph->info->num_philos)
// {
// 	printf("ova->%d\neat_count->%d\n", ph[i].id, ph[i].eat_count);
// 	i++;
// }

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
		if (for_eat_2(ph) == 1)
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

