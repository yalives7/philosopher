// start.c
#include "philo.h"

static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine,
				&data->philos[i]) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	return (0);
}

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

void	start_simulation(t_data *data)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
	{
		perror("Failed to create monitor thread");
		return ;
	}
	if (create_threads(data) != 0)
		return ;
	join_threads(data);
	pthread_join(monitor, NULL); // <-- EKLENDİ
}
