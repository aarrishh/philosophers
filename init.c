/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:45:00 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/15 15:20:02 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_negative_number(t_info *info)
{
	if (info->num_philos < 0 || info->time_to_die < 0
		|| info->time_to_eat < 0 || info->time_to_sleep < 0)
		return (printf("Error: Invalid number\n"), -1);
	else if (info->flag == 1 && info->eat_limit < 0)
		return (printf("Error: Invalid number\n"), -1);
	if (info->time_to_eat > 100000000)
		return (printf("Error: time_to_eat too large\n"), -1);
	return (0);
}

int	init_info(char **argv, t_info *info)
{
	info->flag = 1;
	info->num_philos = check_atoi(ft_atoi(argv[1]));
	if (info->num_philos > MAX_PHILOS)
		return (printf("Error: Too many philos (max %d)\n", MAX_PHILOS), -1);
	info->time_to_die = check_atoi(ft_atoi(argv[2]));
	info->time_to_eat = check_atoi(ft_atoi(argv[3]));
	info->time_to_sleep = check_atoi(ft_atoi(argv[4]));
	if (argv[5])
		info->eat_limit = check_atoi(ft_atoi(argv[5]));
	else
	{
		info->eat_limit = -1;
		info->flag = 0;
	}
	if (check_negative_number(info) == -1)
		return (-1);
	info->start_time = current_timestamp_ms();
	info->is_alive = 1;
	pthread_mutex_init(&info->alive_mutex, NULL);
	pthread_mutex_init(&info->check_eat_count_mutex, NULL);
	info->done_eating = 0;
	return (info->num_philos);
}

void	init_philo(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].eat_count = 0;
		philos[i].last_meal_time = info->start_time;
		philos[i].info = info;
		philos[i].left_fork = &info->forks[i];
		philos[i].right_fork = &info->forks[(i + 1) % info->num_philos];
		pthread_create(&philos[i].thread_id, NULL, thread_function, &philos[i]);
		pthread_create(&philos[i].life_thread_id, NULL, for_life, &philos[i]);
		i++;
	}
}
