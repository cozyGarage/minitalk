/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:50:42 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/14 18:43:37 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static char g_last_char;

void	exit_with_error(const char *msg)
{
	ft_printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

static void	c_handler(int sig)
{
	static int bits_received;
	static int total;

	bits_received = 0;
	total = 0;
	if (sig == SIGUSR1)
	{
		bits_received++;
		if (bits_received == CHAR_BIT)
		{
			if (g_last_char != '\0')
				DEBUG_PRINT("character %d: '%c'\n", total - 3, g_last_char);
			total++;
			bits_received = 0;
		}
	}
	else if (sig == SIGUSR2)
	{
		bits_received++;
		if (bits_received == CHAR_BIT)
		{
			DEBUG_PRINT("The server confirmed the end of the message\n");
			exit(EXIT_SUCCESS);
		}
	}
}

static void	send_mes(int server_pid, char *str)
{
	int i;
	
	i = 0;
	ft_printf("sending length = [%d]\n", ft_strlen(str));
	send_int(server_pid, ft_strlen(str));
	ft_printf("sending message\n");
	while (str[i] != '\0')
	{
		g_last_char = str[i];
		send_char(server_pid, str[i]);
		i++;
	}
	ft_printf("sending null string terminator\n");
	g_last_char = '\0';
	send_char(server_pid, '\0');
}

int	main(int argc, char **argv)
{	
	struct sigaction	sa;

	if (argc != 3)
		exit_with_error("usage: ./client <server PID> <string>");
	else if (kill(ft_atoi(argv[1]), 0) < 0)
		exit_with_error("PID is invalid");
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = c_handler;
	sa.sa_flags = SA_RESTART;
	sigaction_configure(&sa);
	send_mes(ft_atoi(argv[1]), argv[2]);
	return (EXIT_SUCCESS);
}
