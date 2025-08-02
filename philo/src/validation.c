/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:41:48 by sungor            #+#    #+#             */
/*   Updated: 2025/08/02 01:57:18 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	validate_basic_args(char **arg)
{
	if (ft_atoi(arg[1]) <= 0 || check_arg_content(arg[1]) == 1)
	{
		print_error("Error: Invalid number of philosophers"
			" (must be positive number)");
		return (1);
	}
	if (ft_atoi(arg[2]) <= 0 || check_arg_content(arg[2]) == 1)
	{
		print_error("Error: Invalid time to die"
			" (must be positive number)");
		return (1);
	}
	return (0);
}

int	validate_timing_args(char **arg)
{
	if (ft_atoi(arg[3]) <= 0 || check_arg_content(arg[3]) == 1)
	{
		print_error("Error: Invalid time to eat (must be positive number)");
		return (1);
	}
	if (ft_atoi(arg[4]) <= 0 || check_arg_content(arg[4]) == 1)
	{
		print_error("Error: Invalid time to sleep"
			" (must be positive number)");
		return (1);
	}
	if (arg[5] && (ft_atoi(arg[5]) <= 0 || check_arg_content(arg[5]) == 1))
	{
		print_error("Error: Invalid must_eat_count"
			" (must be positive number)");
		return (1);
	}
	return (0);
}

int	validate_arguments(char **arg)
{
	if (validate_basic_args(arg))
		return (1);
	if (validate_timing_args(arg))
		return (1);
	return (0);
}
