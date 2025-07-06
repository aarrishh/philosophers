/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:56:26 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/06 19:04:41 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	check_atoi(long long num)
{
	if ((num < INT_MIN || num > INT_MAX) || num <= 0)
	{
		printf("Error\n");
		exit(1);
	}
	return (num);
}

int	init_info(char **argv, t_info *info)
{
	int num_philos;
	
	num_philos = check_atoi(ft_atoi(argv[1]));
	info->time_to_die = check_atoi(ft_atoi(argv[2]));
	info->time_to_eat = check_atoi(ft_atoi(argv[3]));
	info->time_to_sleep = check_atoi(ft_atoi(argv[4]));
	if (argv[5])
		info->eat_limit = check_atoi(ft_atoi(argv[5]));
	return (num_philos);
}
void*	thread_function(void *philo)
{
	// (void)philo;
	t_philo *Ph = philo;
	// printf("id = %lu\n", Ph->thread_id);
	// printf("id2 = %lu\n", Ph->thread_id2);
	pthread_mutex_lock(&Ph->boo);
	int i = 0;
	while (i < 10000000)
	{
		Ph->bo++;
		i++;
	}
	pthread_mutex_unlock(&Ph->boo);
	printf("vvvvvvv = %d\n", Ph->bo);

	return (NULL);	
}

void	init_philo(t_philo *philo, t_info info, int num_philos)
{
	(void)num_philos;

	// printf("bo = %d\n", philo->bo);

	pthread_mutex_init(&philo->boo, NULL);
		
			pthread_create(&philo->thread_id, NULL, thread_function, philo);
			pthread_create(&philo->thread_id2, NULL, thread_function, philo);

			// philo->bo++;
		// pthread_create(&philo->thread_id2, NULL, thread_function, philo);

			pthread_join(philo->thread_id, NULL);
			pthread_join(philo->thread_id2, NULL);

		// pthread_join(philo->thread_id2, NULL);


	printf("bo = %d\n", philo->bo);
	(void)info;
}

void	validation(char **argv)
{
	t_info info;
	t_philo philo;
	philo.bo = 0;
	int num_philos;
	
	num_philos = init_info(argv, &info);
	init_philo(&philo, info, num_philos);
	// simulation(&philo);
	// printf("argv[1]->%d\n", info.num_philos);
	// printf("argv[2]->%d\n", info.time_to_die);
	// printf("argv[3]->%d\n", info.time_to_eat);
	// printf("argv[4]->%d\n", info.time_to_sleep);
	// if (argv[5])
	// 	printf("argv[5]->%d\n", info.eat_limit);
}


void	simulation(t_philo *philo)
{
	pthread_create(&philo->thread_id, NULL, thread_function, philo);
	// sleep(5);
	pthread_create(&philo->thread_id, NULL, thread_function, philo);
	sleep(1);
	
	// printf("id = %lu\n", philo->thread_id);
}

int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		validation(argv);
	}
	else
		return(printf("Usage: n_philos, t_die, t_eat, t_sleep, [eat_limit]\n"), -1);
}
