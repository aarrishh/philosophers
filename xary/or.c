#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

typedef struct s_philo
{
	int bo;
} t_philo;

void run_process(t_philo *ph)
{
	ph->bo++;
	printf("Child process (pid=%d): bo = %d\n", getpid(), ph->bo);
}

int main(void)
{
	t_philo philo;
	philo.bo = 0;

	pid_t pid1 = fork();
	if (pid1 == 0)
	{
		run_process(&philo);
		printf("nayii\n");
		return 0;
	}
	printf("nayiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");

	pid_t pid2 = fork();
	if (pid2 == 0)
	{
		philo.bo +=5;
		run_process(&philo);
		return 0;
	}

	// wait for both children
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	// In parent process
	printf("Parent process (pid=%d): bo = %d\n", getpid(), philo.bo);
	return 0;
}
