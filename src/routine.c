// routine.c
#include "philo.h"

static int	grab_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	first_fork = philo->left_fork;
	second_fork = philo->right_fork;
	if (philo->id % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	if (pthread_mutex_lock(first_fork) != 0)
		return (0);
	print_philo_action(philo, "has taken a fork");
	if (philo->data->num_philos == 1)
	{
		smart_sleep(philo->data->time_to_die * 2);
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	if (pthread_mutex_lock(second_fork) != 0)
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	print_philo_action(philo, "has taken a fork");
	return (1);
}

static void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	eat_meal(t_philo *philo)
{
	print_philo_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->is_eating = 1;
	philo->last_meal = get_timestamp_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	smart_sleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->is_eating = 0;
	pthread_mutex_unlock(&philo->data->death_mutex);
}

static int	check_meal_count(t_philo *philo)
{
	int	should_exit;

	pthread_mutex_lock(&philo->data->death_mutex);
	should_exit = 0;
	if (philo->data->must_eat_count > 0
		&& philo->meals_eaten >= philo->data->must_eat_count)
		should_exit = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (should_exit);
}

static void	handle_single_philosopher(t_philo *philo)
{
	grab_forks(philo);
}

static void	philosopher_cycle(t_philo *philo)
{
	if (has_simulation_ended(philo->data))
		return ;
	if (!grab_forks(philo))
		return ;
	eat_meal(philo);
	release_forks(philo);
	if (check_meal_count(philo) || has_simulation_ended(philo->data))
		return ;
	print_philo_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep);
	if (has_simulation_ended(philo->data))
		return ;
	usleep(1000);
}

void	perform_routine(t_philo *philo)
{
	if (philo->data->num_philos == 1)
	{
		handle_single_philosopher(philo);
		return ;
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!has_simulation_ended(philo->data))
	{
		philosopher_cycle(philo);
		if (check_meal_count(philo) || has_simulation_ended(philo->data))
			break ;
	}
}
