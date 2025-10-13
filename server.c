/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:32:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/13 22:19:11 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
  Check if the string message is finished.
  In case the null terminator string is received, prints the whole string
  and frees the string from heap memory, then informs the client that the message was
  received (client will then exit) using send_bit() without expecting a signal 
  to be received back from the client.
*/
static void	server_is_message_finished(
	t_protocol *t_server, int *i, pid_t client_pid)
{
	if (t_server->bits == CHAR_BIT && t_server->flag == 1)
	{
		t_server->message[*i] = t_server->data;
		(*i)++;
		if (t_server->data == '\0')
		{
			ft_printf("%s\n", t_server->message);
			free(t_server->message);
			t_server->message = NULL;
			t_server->flag = 0;
			*i = 0;
			DEBUG_PRINT("[DEBUG] End of message received\n");
			send_bit(client_pid, 1, 0);
		}
		t_server->bits = 0;
	}
}

/*
  Checks if the string length bits are received.
  If yes, the length is printed to stdout and used to allocate in the 
  heap memory a string with the exact size received (plus the null terminator).
*/
static void	init_struct(t_protocol *t_server)
{
	if (t_server->bits == sizeof(int) * CHAR_BIT && t_server->flag == 0)
	{
		t_server->flag = 1;
		t_server->message = ft_calloc(t_server->data + 1, sizeof(char));
		if (!t_server->message)
			exit_with_error("Error: memory allocation failed");
		t_server->message[t_server->data] = '\0';
		t_server->bits = 0;
	}
}
/*
  Signal handler using sa_sigaction for extensibility. Catches SIGUSR1 and 
  SIGUSR2 signals received from the client that represent the status of each 
  bit (either 0 or 1).
  
  Variables declared as static are initialized with zero, and after each 
  8 bits received they are again re-initialized to zero.
  
  For each bit received from the client, the server sends an ACK signal
  (then the client should only send the next bit after this ACK signal).
*/
static void	s_handler(int num, siginfo_t *info, void *context)
{
	static t_protocol	t_server;
	static int				i;

	usleep(WAIT_US);
	(void)context;
	if (t_server.bits == 0)
		t_server.data = 0;
	if (num == SIGUSR2 && t_server.flag == 0)
		t_server.data |= 1 << (((sizeof(int) * CHAR_BIT) - 1) - t_server.bits);
	else if (num == SIGUSR2 && t_server.flag == 1)
		t_server.data |= 1 << ((CHAR_BIT - 1) - t_server.bits);
	DEBUG_PRINT("[DEBUG] Received bit %d\n", t_server.bits);
	t_server.bits++;
	init_struct(&t_server);
	server_is_message_finished(&t_server, &i, info->si_pid);
	send_bit(info->si_pid, 0, 0);
}

/*
  This program (server) prints to stdout the PID process and keeps
  listening for incoming message transmissions. It uses sigaction()
  to catch the signals sent by the client.
*/
int	main(void)
{
	struct sigaction s_server;

	sigemptyset(&s_server.sa_mask);
	s_server.sa_sigaction = s_handler;
	s_server.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction_configure(&s_server);
	ft_printf("server [PID = %d]\n", getpid());
	while (1)
		pause();
	return (EXIT_SUCCESS);
}