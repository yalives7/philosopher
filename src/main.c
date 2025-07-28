// Argument içeriğinin sadece sayı olup olmadığını kontrol eder
#include "philo.h"

static t_data	*g_data = NULL;

static void	signal_handler(int sig)
{
	(void)sig;
	if (g_data)
	{
		pthread_mutex_lock(&g_data->death_mutex);
		g_data->simulation_end = 1;
		pthread_mutex_unlock(&g_data->death_mutex);
	}
}

static int	check_arg_content(char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
		return (1);
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static int	validate_arguments(char **arg)
{
	if (atoi(arg[1]) <= 0 || check_arg_content(arg[1]) == 1)
	{
		printf("Error: Invalid number of philosophers"
				" (must be positive number)\n");
		return (1);
	}
	if (atoi(arg[2]) <= 0 || check_arg_content(arg[2]) == 1)
	{
		printf("Error: Invalid time to die (must be positive number)\n");
		return (1);
	}
	if (atoi(arg[3]) <= 0 || check_arg_content(arg[3]) == 1)
	{
		printf("Error: Invalid time to eat (must be positive number)\n");
		return (1);
	}
	if (atoi(arg[4]) <= 0 || check_arg_content(arg[4]) == 1)
	{
		printf("Error: Invalid time to sleep (must be positive number)\n");
		return (1);
	}
	if (arg[5] && (atoi(arg[5]) <= 0 || check_arg_content(arg[5]) == 1))
	{
		printf("Error: Invalid must_eat_count (must be positive number)\n");
		return (1);
	}
	return (0);
}

int	parse_args(int ac, char **arg, t_data *data)
{
	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]\n");
		return (1);
	}
	if (validate_arguments(arg))
		return (1);
	data->num_philos = atoi(arg[1]);
	data->time_to_die = atoi(arg[2]);
	data->time_to_eat = atoi(arg[3]);
	data->time_to_sleep = atoi(arg[4]);
	if (ac == 6)
		data->must_eat_count = atoi(arg[5]);
	else
		data->must_eat_count = -1;
	data->simulation_end = 0;
	return (0);
}

int	main(int ac, char **arg)
{
	t_data	data;

	if (parse_args(ac, arg, &data))
		return (1);
	if (init_all(&data))
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	
	// Global pointer set et ve signal handler kaydet
	g_data = &data;
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	
	start_simulation(&data);
	clean_up(&data);
	return (0);
}
