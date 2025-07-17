/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:50:18 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/17 19:31:56 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_message(t_philo *ph, char *text)
{
	long long	timestamp;

	pthread_mutex_lock(&ph->info->alive_mutex);
	if (ph->info->is_alive == -1)
	{
		pthread_mutex_unlock(&ph->info->alive_mutex);
		return (0);
	}
	timestamp = current_timestamp_ms() - ph->info->start_time;
	printf("[%lld] %d %s\n", timestamp, ph->id, text);
	pthread_mutex_unlock(&ph->info->alive_mutex);
	return (1);
}

void	case_one(t_philo *ph)
{
	long long	timestamp;

	pthread_mutex_lock(ph->left_fork);
	print_message(ph, "has taken a fork");
	usleep(ph->info->time_to_die * 1000);
	pthread_mutex_lock(&ph->info->alive_mutex);
	if (ph->info->is_alive == 1)
	{
		ph->info->is_alive = -1;
		timestamp = current_timestamp_ms() - ph->info->start_time;
		printf("[%lld] %d died\n", timestamp, ph->id);
	}
	pthread_mutex_unlock(&ph->info->alive_mutex);
	pthread_mutex_unlock(ph->left_fork);
}

void	validation_2(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	init_philo(philos, info);
	if (info->eat_limit != -1)
		pthread_create(&info->eat_thread_id, NULL, for_eat, philos);
	simulation(philos, info);
	while (i < info->num_philos)
	{
		pthread_mutex_destroy(&info->forks[i]);
		pthread_mutex_destroy(&philos[i].last_meal_time_mutex);
		i++;
	}
	destroy_function(philos, info);
	free(info->forks);
	free(philos);
}

int	validation(char **argv)
{
	t_philo	*philos;
	t_info	info;
	int		i;

	philos = NULL;
	info.num_philos = init_info(argv, &info, philos);
	if (info.num_philos == -1)
		return (destroy_function(philos, &info), 1);
	info.forks = malloc(sizeof(pthread_mutex_t) * info.num_philos);
	if (!info.forks)
	{
		printf("Malloc failed (forks)\n");
		return (destroy_function(philos, &info), 1);
	}
	i = 0;
	while (i < info.num_philos)
		pthread_mutex_init(&info.forks[i++], NULL);
	philos = malloc(sizeof(t_philo) * info.num_philos);
	if (!philos)
	{
		printf("Malloc failed (philosophers)\n");
		return (destroy_function(philos, &info), 1);
	}
	validation_2(philos, &info);
	return (0);
}

int	main(int argc, char **argv)
{
	int	check;

	check = 0;
	if (argc == 5 || argc == 6)
	{
		check = validation(argv);
		if (check == 1)
			return (1);
	}
	else
		return (printf("Usage: n_philos t_die t_eat t_sleep [eat_limit]\n"), 1);
	return (0);
}
