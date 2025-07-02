// monitor.c
#include "philo.h"

static int	check_full_philosopher(t_data *data, int i)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->must_eat_count > 0
		&& data->philos[i].meals_eaten >= data->must_eat_count)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	if (data->philos[i].is_eating)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (-1);
}

static int	handle_philosopher_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->death_mutex);
	data->simulation_end = 1;
	data->dead_flag = 1;
	pthread_mutex_unlock(&data->death_mutex);
	pthread_mutex_lock(&data->print_mutex);
	printf("%lld %d died\n", get_timestamp_ms() - data->start_time,
		data->philos[i].id);
	pthread_mutex_unlock(&data->print_mutex);
	return (-1);
}

static int	check_philosopher_death(t_data *data, int i)
{
	long long	time_since_last_meal;
	int			status;

	status = check_full_philosopher(data, i);
	if (status != -1)
		return (status);
	pthread_mutex_lock(&data->death_mutex);
	time_since_last_meal = get_timestamp_ms() - data->philos[i].last_meal;
	pthread_mutex_unlock(&data->death_mutex);
	if (time_since_last_meal >= data->time_to_die)
		return (handle_philosopher_death(data, i));
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		full_philos;
	int		result;

	data = (t_data *)arg;
	while (!has_simulation_ended(data))
	{
		full_philos = 0;
		for (i = 0; i < data->num_philos; i++)
		{
			result = check_philosopher_death(data, i);
			if (result == -1)
				return (NULL); // Ölüm gerçekleşti
			if (result == 1)
				full_philos++; // Doymuş filozof
		}
		// Herkes yeterince yediyse bitir
		if (data->must_eat_count > 0 && full_philos == data->num_philos)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
		usleep(1000); // 1ms kontrol - daha hassas ama performanslı
	}
	return (NULL);
}

