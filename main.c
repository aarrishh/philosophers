/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arina <arina@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:56:26 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/07 17:13:32 by arina            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>


long long current_timestamp_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

long	check_atoi(long long num)
{
	if ((num < INT_MIN || num > INT_MAX) || num <= 0)
	{
		printf("Error: Invalid number\n");
		exit(1);
	}
	return (num);
}

int	init_info(char **argv, t_info *info)
{
	int num_philos;

	num_philos = check_atoi(ft_atoi(argv[1]));
	if (num_philos > MAX_PHILOS)
		return (printf("Error: Too many philosophers (max %d)\n", MAX_PHILOS), -1);
	info->time_to_die = check_atoi(ft_atoi(argv[2]));
	info->time_to_eat = check_atoi(ft_atoi(argv[3]));
	info->time_to_sleep = check_atoi(ft_atoi(argv[4]));
	info->eat_limit = -1;
	if (argv[5])
		info->eat_limit = check_atoi(ft_atoi(argv[5]));
	info->num_philos = num_philos;
	return (num_philos);
}

void print_state(t_philo *philo, const char *msg)
{
	long long timestamp = current_timestamp_ms() - philo->info->start_time;
	printf("[%lld] %d %s\n", timestamp, philo->id, msg);
}

void	*thread_function(void *arg)
{
	t_philo *ph = (t_philo *)arg;

	pthread_mutex_lock(ph->left_fork);
	print_state(ph, "has taken a fork");

	pthread_mutex_lock(ph->right_fork);
	print_state(ph, "has taken a fork");

	print_state(ph, "is eating");
	usleep(ph->info->time_to_eat * 1000);

	pthread_mutex_unlock(ph->right_fork);
	pthread_mutex_unlock(ph->left_fork);

	print_state(ph, "is sleeping");
	usleep(ph->info->time_to_sleep * 1000);

	print_state(ph, "is thinking");

	return (NULL);
}

void	init_philo(t_philo *philos, t_info *info, int num_philos)
{
	int i = 0;

	while (i < num_philos)
	{
		philos[i].id = i + 1;
		philos[i].eaten_count = 0;
		philos[i].is_alive = 1;
		philos[i].info = info;
		philos[i].left_fork = &info->forks[i];
		philos[i].right_fork = &info->forks[(i + 1) % num_philos];

		pthread_create(&philos[i].thread_id, NULL, thread_function, &philos[i]);
		i++;
	}
}

void	simulation(t_philo *philos, int num_philos)
{
	int i = 0;

	while (i < num_philos)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
}

void	validation(char **argv)
{
	t_info info;
	t_philo *philos;
	int num_philos;
	int i;

	num_philos = init_info(argv, &info);
	info.forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (!info.forks)
		return (printf("Malloc failed (forks)\n"), -1);
	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_init(&info.forks[i], NULL);
		i++;
	}
	philos = malloc(sizeof(t_philo) * num_philos);
	if (!philos)
	{
		printf("Malloc failed (philosophers)\n");
		exit(1);
	}
	info.start_time = current_timestamp_ms();
	init_philo(philos, &info, num_philos);
	simulation(philos, num_philos);
	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_destroy(&info.forks[i]);
		i++;
	}

	free(info.forks);
	free(philos);
}

int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
		validation(argv);
	else
		return (printf("Usage: n_philos t_die t_eat t_sleep [eat_limit]\n"), -1);
	return (0);
}
