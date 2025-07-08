/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:55:22 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/08 18:00:56 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MAX_PHILOS
#define MAX_PHILOS 200
#endif

typedef struct s_philo t_philo;
typedef struct s_info
{
	int num_philos;
	int is_alive;
	pthread_mutex_t alive_mutex;
	long long	start_time;
	int	time_to_eat;
	int	time_to_die;
	int	time_to_sleep;
	int	eat_limit;
	pthread_mutex_t *forks;
	pthread_mutex_t	print;
	
	t_philo *philos;
} t_info;

typedef struct s_philo
{
	int	id;
	pthread_mutex_t	meal_mutex;
	long long last_meal_time;
	pthread_t	thread_id;
	pthread_t	thread_id2;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	
	t_info	*info;
} t_philo;

//Libft functions
long long	ft_atoi(const char *str);
void	simulation(t_philo *philos, int num_philos);
#endif
