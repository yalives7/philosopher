// monitor.c
#include "philo.h"

static int	is_philosopher_dead(t_philo *philo, t_data *data)
{
	long long	time_since_last_meal;
	long long	current_time;

	pthread_mutex_lock(&data->death_mutex);
	if (philo->is_eating)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (0);
	}
	current_time = get_timestamp_ms();
	time_since_last_meal = current_time - philo->last_meal;
	if (time_since_last_meal > data->time_to_die)
	{
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d %s\n", current_time - data->start_time,
			philo->id, "died");
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

static int	are_all_philos_full(t_data *data)
{
	int	i;
	int	full_count;

	if (data->must_eat_count <= 0)
		return (0);
	full_count = 0;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->philos[i].meals_eaten >= data->must_eat_count)
			full_count++;
		pthread_mutex_unlock(&data->death_mutex);
		i++;
	}
	if (full_count == data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (is_philosopher_dead(&data->philos[i], data))
				return (NULL);
			i++;
		}
		if (are_all_philos_full(data))
			return (NULL);
		if (has_simulation_ended(data))
			break ;
		usleep(500); // Daha kısa aralık
	}
	return (NULL);
}

