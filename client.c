/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:50:42 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/14 21:11:12 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	c_handler(int sig)
{
	if (sig == SIGUSR2)
	{
		exit(EXIT_SUCCESS);
	}
}

static void	send_package(int server_pid, char *str)
{
	int		i;

	i = 0;
	ping_int(server_pid, ft_strlen(str));
	while (str[i] != '\0')
		ping_char(server_pid, str[i++]);
	ping_char(server_pid, '\0');
}

int	main(int argc, char **argv)
{
	struct sigaction	s_c;

	if (argc != 3)
		exit_with_error("usage: ./client <server PID> <string>");
	else if (kill(ft_atoi(argv[1]), 0) < 0)
		exit_with_error("PID is invalid");
	sigemptyset(&s_c.sa_mask);
	s_c.sa_handler = c_handler;
	s_c.sa_flags = SA_RESTART;
	sigaction_configure(&s_c);
	send_package(ft_atoi(argv[1]), argv[2]);
	return (EXIT_SUCCESS);
}
