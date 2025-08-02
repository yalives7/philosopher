/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:40:56 by sungor            #+#    #+#             */
/*   Updated: 2025/08/02 01:56:39 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <signal.h>

static t_data	**get_data_ptr(void)
{
	static t_data	*data = NULL;

	return (&data);
}

static void	signal_handler(int sig)
{
	t_data	**data_ptr;
	t_data	*data;

	(void)sig;
	data_ptr = get_data_ptr();
	data = *data_ptr;
	if (data)
	{
		safe_mutex_lock(&data->death_mutex);
		data->simulation_end = 1;
		safe_mutex_unlock(&data->death_mutex);
	}
}

int	parse_args(int ac, char **arg, t_data *data)
{
	if (ac != 5 && ac != 6)
	{
		print_error("Usage: ./philo <num_philos> <time_to_die> "
			"<time_to_eat> <time_to_sleep> [must_eat_count]");
		return (1);
	}
	if (validate_arguments(arg))
		return (1);
	data->num_philos = ft_atoi(arg[1]);
	data->time_to_die = ft_atoi(arg[2]);
	data->time_to_eat = ft_atoi(arg[3]);
	data->time_to_sleep = ft_atoi(arg[4]);
	if (ac == 6)
		data->must_eat_count = ft_atoi(arg[5]);
	else
		data->must_eat_count = -1;
	data->simulation_end = 0;
	return (0);
}

int	main(int ac, char **arg)
{
	t_data	data;
	t_data	**data_ptr;

	if (parse_args(ac, arg, &data))
		return (1);
	if (init_all(&data))
	{
		print_error("Error: Initialization failed");
		return (1);
	}
	data_ptr = get_data_ptr();
	*data_ptr = &data;
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	start_simulation(&data);
	clean_up(&data);
	return (0);
}
