/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:35:31 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/12 15:39:39 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*thread_function_2(t_philo *ph)
{
	if (!print_message(ph, "has taken a fork"))
	{
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(ph->left_fork);
		return NULL;
	}
	if (!print_message(ph, "is eating"))
	{
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(ph->left_fork);
		return NULL;
	}
	pthread_mutex_lock(&ph->info->check_eat_count_mutex);
	ph->eat_count += 1;
	pthread_mutex_unlock(&ph->info->check_eat_count_mutex);
	ph->last_meal_time = current_timestamp_ms();
	usleep(ph->info->time_to_eat * 1000);
	pthread_mutex_unlock(ph->right_fork);
	pthread_mutex_unlock(ph->left_fork);
	if (!print_message(ph, "is sleeping"))
		return NULL;
	usleep(ph->info->time_to_sleep * 1000);
	if (!print_message(ph, "is thinking"))
		return NULL;
	return "";
}

void	*thread_function(void *philo)
{
	t_philo *ph = (t_philo *)philo;

	if (ph->id % 2 == 0)
		usleep(500);
	if (ph->info->num_philos == 1)
		return (case_one(ph), NULL);
	while (1)
	{
		if (check_alive(ph) == -1)
			return NULL;
		pthread_mutex_lock(ph->left_fork);
		if (!print_message(ph, "has taken a fork"))
		{
			pthread_mutex_unlock(ph->left_fork);
			return NULL;
		}
		pthread_mutex_lock(ph->right_fork);
		thread_function_2(ph);
	}
	return NULL;
}

void	*for_life(void *philo)
{
	t_philo *ph = (t_philo *)philo;
	while (1)
	{
		usleep(1000);
		if (check_alive(ph) == -1)
			return NULL;
		if (current_timestamp_ms() - ph->last_meal_time > ph->info->time_to_die)
		{
			pthread_mutex_lock(&ph->info->alive_mutex); 
			if (ph->info->is_alive == 1)
			{
				ph->info->is_alive = -1;
				long long timestamp = current_timestamp_ms() - ph->info->start_time;
				printf("[%lld] %d died\n", timestamp, ph->id);
			}
			pthread_mutex_unlock(&ph->info->alive_mutex);
			return NULL;
		}
	}
	return NULL;
}

void simulation(t_philo *philos, t_info *info)
{
	int i = 0;
	while (i < info->num_philos)
	{
		pthread_join(philos[i].thread_id, NULL);
		pthread_join(philos[i].life_thread_id, NULL);
		i++;
	}
	if (info->eat_limit != -1)
		pthread_join(info->eat_thread_id, NULL);
}

