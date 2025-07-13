/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arimanuk <arimanuk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:55:22 by arimanuk          #+#    #+#             */
/*   Updated: 2025/07/12 17:18:03 by arimanuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# ifndef MAX_PHILOS
#  define MAX_PHILOS 200
# endif

typedef struct s_philo	t_philo;
typedef struct s_info
{
	int				num_philos;
	int				time_to_eat;
	int				time_to_die;
	int				eat_limit;
	int				time_to_sleep;
	int				is_alive;
	int				done_eating;
	long long		start_time;
	pthread_t		eat_thread_id;
	pthread_mutex_t	check_eat_count_mutex;
	pthread_mutex_t	alive_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_info;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long long		last_meal_time;
	pthread_t		thread_id;
	pthread_t		life_thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_info			*info;
}	t_philo;

//Libft functions
int			print_message(t_philo *ph, char *text);
int			init_info(char **argv, t_info *info);
int			check_alive(t_philo *ph);
int			for_eat_2(t_philo *ph);
long long	current_timestamp_ms(void);
long long	ft_atoi(const char *str);
long		check_atoi(long long num);
void		*for_eat(void *philo);
void		case_one(t_philo *ph);
void		*for_life(void *philo);
void		*thread_function(void *philo);
void		init_philo(t_philo *philos, t_info *info);
void		simulation(t_philo *philos, t_info *info);
void		validation_2(t_philo *philos, t_info *info);
#endif
