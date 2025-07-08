/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arina <arina@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:55:22 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/07 17:03:03 by arina            ###   ########.fr       */
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
	int is_alive;
	int eaten_count;
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
