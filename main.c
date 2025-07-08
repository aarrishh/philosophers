#include "philo.h"
#include <sys/time.h>

long long current_timestamp_ms(void)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000LL + time.tv_usec / 1000);
}

long check_atoi(long long num)
{
	if ((num < INT_MIN || num > INT_MAX) || num <= 0)
	{
		printf("Error: Invalid number\n");
		exit(1);
	}
	return (num);
}

int init_info(char **argv, t_info *info)
{
	int num_philos;

	num_philos = check_atoi(ft_atoi(argv[1]));
	if (num_philos > MAX_PHILOS)
		return (printf("Error: Too many philosophers (max %d)\n", MAX_PHILOS), -1);
	info->time_to_die = check_atoi(ft_atoi(argv[2]));
	info->time_to_eat = check_atoi(ft_atoi(argv[3]));
	info->time_to_sleep = check_atoi(ft_atoi(argv[4]));
	info->start_time = current_timestamp_ms();
	info->is_alive = 1;
	pthread_mutex_init(&info->alive_mutex, NULL);
	info->eat_limit = -1;
	if (argv[5])
		info->eat_limit = check_atoi(ft_atoi(argv[5]));
	info->num_philos = num_philos;
	return (num_philos);
}

int safe_print(t_philo *ph, char *text) // ✅ ՆՈՐ ՖՈՒՆԿՑԻԱ՝ վերահսկելու տպումը
{
	pthread_mutex_lock(&ph->info->alive_mutex);
	if (ph->info->is_alive == -1)
	{
		pthread_mutex_unlock(&ph->info->alive_mutex);
		return 0; // ✅ ՆՈՐ՝ չտպի, եթե մեկը արդեն մեռել է
	}
	long long timestamp = current_timestamp_ms() - ph->info->start_time;
	printf("[%lld] %d %s\n", timestamp, ph->id, text);
	pthread_mutex_unlock(&ph->info->alive_mutex);
	return 1;
}

void case_one(t_philo *ph)
{
	pthread_mutex_lock(ph->left_fork);
	safe_print(ph, "has taken a fork"); // ✅ ՓՈԽԱՐԻՆԵՑԻ print_state safe_print-ով
	usleep(ph->info->time_to_die * 1000);
	pthread_mutex_lock(&ph->info->alive_mutex); // ✅ Ավելացվել է մեռնելուց առաջ lock
	if (ph->info->is_alive == 1)
	{
		ph->info->is_alive = -1;
		long long timestamp = current_timestamp_ms() - ph->info->start_time;
		printf("[%lld] %d died\n", timestamp, ph->id); // ✅ Մենակ մեկ death տպելու համար
	}
	pthread_mutex_unlock(&ph->info->alive_mutex);
	pthread_mutex_unlock(ph->left_fork);
}

int check_alive(t_philo *ph) // ✅ ՆՈՐ ՖՈՒՆԿՑԻԱ՝ ստուգում է արդյոք simulation-ը շարունակվի
{
	pthread_mutex_lock(&ph->info->alive_mutex);
	if (ph->info->is_alive == -1)
	{
		pthread_mutex_unlock(&ph->info->alive_mutex);
		return -1; // ✅ Եթե արդեն մեռած է՝ դադարեցնել
	}
	pthread_mutex_unlock(&ph->info->alive_mutex);
	return 0;
}

void *thread_function(void *philo)
{
	t_philo *ph = (t_philo *)philo;

	while (1)
	{
		if (check_alive(ph) == -1) // ✅ Ավելացված է՝ ամեն ցիկլի սկզբում ստուգի
			return NULL;
		if (ph->id % 2 == 0)
			usleep(500);
		if (ph->info->num_philos == 1)
			return (case_one(ph), NULL);
		pthread_mutex_lock(ph->left_fork);
		if (!safe_print(ph, "has taken a fork")) // ✅ safe_print ստուգում
		{
			pthread_mutex_unlock(ph->left_fork);
			return NULL;
		}
		pthread_mutex_lock(ph->right_fork);
		if (!safe_print(ph, "has taken a fork")) // ✅ safe_print ստուգում
		{
			pthread_mutex_unlock(ph->right_fork);
			pthread_mutex_unlock(ph->left_fork);
			return NULL;
		}
		if (!safe_print(ph, "is eating")) // ✅ safe_print ստուգում
		{
			pthread_mutex_unlock(ph->right_fork);
			pthread_mutex_unlock(ph->left_fork);
			return NULL;
		}
		ph->last_meal_time = current_timestamp_ms();
		usleep(ph->info->time_to_eat * 1000);
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(ph->left_fork);
		if (!safe_print(ph, "is sleeping")) // ✅ safe_print ստուգում
			return NULL;
		usleep(ph->info->time_to_sleep * 1000);
		if (!safe_print(ph, "is thinking")) // ✅ safe_print ստուգում
			return NULL;
		if (current_timestamp_ms() - ph->last_meal_time > ph->info->time_to_die)
		{
			pthread_mutex_lock(&ph->info->alive_mutex);
			if (ph->info->is_alive == 1) // ✅ Ստուգում՝ մեռած չլինի
			{
				ph->info->is_alive = -1;
				long long timestamp = current_timestamp_ms() - ph->info->start_time;
				printf("[%lld] %d died\n", timestamp, ph->id); // ✅ Մենակ մեկ death
			}
			pthread_mutex_unlock(&ph->info->alive_mutex);
			return NULL;
		}
	}
	return NULL;
}

void init_philo(t_philo *philos, t_info *info)
{
	int i = 0;
	while (i < info->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = info->start_time;
		philos[i].info = info;
		philos[i].left_fork = &info->forks[i];
		philos[i].right_fork = &info->forks[(i + 1) % info->num_philos];
		pthread_create(&philos[i].thread_id, NULL, thread_function, &philos[i]);
		i++;
	}
}

void simulation(t_philo *philos, int num_philos)
{
	int i = 0;
	while (i < num_philos)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
}

void validation(char **argv)
{
	t_info info;
	t_philo *philos;
	int i;

	info.num_philos = init_info(argv, &info);
	info.forks = malloc(sizeof(pthread_mutex_t) * info.num_philos);
	if (!info.forks)
	{
		printf("Malloc failed (forks)\n");
		return;
	}
	i = 0;
	while (i < info.num_philos)
	{
		pthread_mutex_init(&info.forks[i], NULL);
		i++;
	}
	philos = malloc(sizeof(t_philo) * info.num_philos);
	if (!philos)
	{
		printf("Malloc failed (philosophers)\n");
		exit(1);
	}
	init_philo(philos, &info);
	simulation(philos, info.num_philos);
	i = 0;
	while (i < info.num_philos)
	{
		pthread_mutex_destroy(&info.forks[i]);
		i++;
	}
	free(info.forks);
	free(philos);
}

int main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
		validation(argv);
	else
		return (printf("Usage: n_philos t_die t_eat t_sleep [eat_limit]\n"), -1);
	return 0;
}
