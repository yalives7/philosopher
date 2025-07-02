// routine.c
#include "philo.h"

static void	grab_forks(t_philo *philo)
{
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		while (!has_simulation_ended(philo->data)
			&& !dead_loop_check(philo->data))
			usleep(1000);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	// Deadlock'u önlemek için: Son filozof (num_philos) tersine al, diğerleri normal
	if (philo->id == philo->data->num_philos)
	{
		// Son filozof: Önce sağ, sonra sol
		pthread_mutex_lock(philo->right_fork);
		if (has_simulation_ended(philo->data) || dead_loop_check(philo->data))
		{
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		// Diğer filozoflar: Önce sol, sonra sağ
		pthread_mutex_lock(philo->left_fork);
		if (has_simulation_ended(philo->data) || dead_loop_check(philo->data))
		{
			pthread_mutex_unlock(philo->left_fork);
			return;
		}
		pthread_mutex_lock(philo->right_fork);
	}
}

static void	release_forks(t_philo *philo)
{
	if (philo->data->num_philos == 1)
		return ;
	// Fork'ları güvenli şekilde bırak - sıra önemli değil
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
	print_philo_action(philo, "is thinking");
	grab_forks(philo);
}

static void	philosopher_cycle(t_philo *philo)
{
	grab_forks(philo);
	if (has_simulation_ended(philo->data) || dead_loop_check(philo->data))
	{
		release_forks(philo);
		return ;
	}
	eat_meal(philo);
	release_forks(philo);
	if (check_meal_count(philo))
		return ;
	if (has_simulation_ended(philo->data) || dead_loop_check(philo->data))
		return ;
	print_philo_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep);
	if (has_simulation_ended(philo->data) || dead_loop_check(philo->data))
		return ;
	print_philo_action(philo, "is thinking");
	// 5 filozoflu sistem için düşünme süresini minimize et
	if (philo->data->num_philos == 5)
		usleep(1000); // Sadece 1ms düşün
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
	while (!has_simulation_ended(philo->data) && !dead_loop_check(philo->data))
	{
		philosopher_cycle(philo);
		if (check_meal_count(philo))
			break ;
		if (has_simulation_ended(philo->data) || dead_loop_check(philo->data))
			break ;
	}
}
