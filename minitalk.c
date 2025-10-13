/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:44:42 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/13 22:09:23 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	exit_with_error(const char *msg)
{
	ft_printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

/*
  Function sets the signals to be caught by the custom handler.
  In case this action fails, prints to stdout an error message and exits the
  program
*/
void	sigaction_configure(struct sigaction *sa)
{
	if (sigaction(SIGUSR1, sa, NULL) < 0)
		exit_with_error("SIGUSR1 is invalid");
	if (sigaction(SIGUSR2, sa, NULL) < 0)
		exit_with_error("SIGUSR2 is invalid");
}

/*
  Functions sends an integer containing the length of the message
  For each bit sent client, waits a signal received back before proceeding
  by using flag = 1 on the send_bit()
  Assumed 1 byte = 8 bits
*/
void send_int(pid_t pid, int num)
{
    send_bits(pid, (unsigned long)num, sizeof(int) * CHAR_BIT);
}

void send_char(pid_t pid, char c)
{
    send_bits(pid, (unsigned long)c, CHAR_BIT);
}

/*
  Function sends a bit (0 or 1) to the process PID
  Return from function will happen after ACK signal is received in case
  the wait flag is set to non zero, otherwise return immediately
*/
void	send_bit(pid_t pid, char bit, char flag_to_pause)
{
	int sig = (bit == 0) ? SIGUSR1 : SIGUSR2;

	if (kill(pid, sig) < 0)
		exit_with_error("Error: sending signal");
	if (flag_to_pause)
		pause();
}

void	send_bits(pid_t pid, unsigned long value, int bit_count)
{
    int shift = bit_count - 1;
    char bit;
    while (shift >= 0)
    {
        bit = (value >> shift) & 1;
        send_bit(pid, bit, 1);
        shift--;
    }
}