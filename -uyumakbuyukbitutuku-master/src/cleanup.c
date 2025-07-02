// cleanup.c
#include "philo.h"

static void	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

static void	destroy_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

void	clean_up(t_data *data)
{
	if (data->philos)
	{
		free(data->philos);
	}
	if (data->forks)
	{
		destroy_forks(data);
	}
	destroy_mutexes(data);
}
