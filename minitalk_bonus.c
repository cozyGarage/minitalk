/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:44:42 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/14 20:58:48 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

void	exit_with_error(const char *msg)
{
	ft_printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

void	sigaction_configure(struct sigaction *sa)
{
	if (sigaction(SIGUSR1, sa, NULL) < 0)
		exit_with_error("SIGUSR1 is invalid");
	if (sigaction(SIGUSR2, sa, NULL) < 0)
		exit_with_error("SIGUSR2 is invalid");
}

void	ping_int(pid_t pid, int nbr)
{
	int		shift;
	char	bit;

	shift = (sizeof(int) * CHAR_BIT) - 1;
	while (shift >= 0)
	{
		bit = (nbr >> shift) & 1;
		ft_ping(pid, bit, 1);
		shift--;
	}
}

void	ping_char(pid_t pid, char c)
{
	int		shift;
	char	bit;

	shift = (sizeof(char) * CHAR_BIT) - 1;
	while (shift >= 0)
	{
		bit = (c >> shift) & 1;
		ft_ping(pid, bit, 1);
		shift--;
	}
}

void	ft_ping(pid_t pid, char bit, int pause_flag)
{
	if (bit == 0)
	{
		if (kill(pid, SIGUSR1) < 0)
			exit_with_error("Error: sending SIGUSR1");
	}
	else if (bit == 1)
	{
		if (kill(pid, SIGUSR2) < 0)
			exit_with_error("Error: sending SIGUSR2");
	}
	if (pause_flag != 0)
		pause();
}
