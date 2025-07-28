/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:40:56 by sungor            #+#    #+#             */
/*   Updated: 2025/07/29 00:19:54 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	signal_handler(int sig)
{
	(void)sig;
	write(2, "Signal received, exiting...\n", 28);
	exit(130);
}

int	parse_args(int ac, char **arg, t_data *data)
{
	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo <num_philos> <time_to_die> ");
		printf("<time_to_eat> <time_to_sleep> [must_eat_count]\n");
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
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	start_simulation(&data);
	clean_up(&data);
	return (0);
}
