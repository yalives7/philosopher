// philo.c
#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	perform_routine(philo);
	return (NULL);
}
