/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:32:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/14 20:34:24 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	decode_signal(t_protocol *t_package, int num)
{
	if (t_package->bits == 0)
		t_package->data = 0;
	if (num == SIGUSR2 && t_package->is_ready == 0)
		t_package->data |= 1 << (((sizeof(int) * 8) - 1) - t_package->bits);
	else if (num == SIGUSR2 && t_package->is_ready == 1)
		t_package->data |= 1 << ((CHAR_BIT - 1) - t_package->bits);
	t_package->bits++;
}

static void	parsing_message(t_protocol *t_package, int *i, pid_t client_pid)
{
	if (t_package->bits == CHAR_BIT && t_package->is_ready == 1)
	{
		t_package->str[*i] = t_package->data;
		(*i)++;
		if (t_package->data == '\0')
		{
			ft_printf("%s\n", t_package->str);
			free(t_package->str);
			t_package->str = NULL;
			t_package->is_ready = 0;
			*i = 0;
			ft_ping(client_pid, 1, 0);
		}
		t_package->bits = 0;
	}
}

static void	init_msg_storage(t_protocol *t_package)
{
	if (t_package->bits == sizeof(int) * CHAR_BIT && t_package->is_ready == 0)
	{
		t_package->is_ready = 1;
		t_package->str = ft_calloc(t_package->data + 1, sizeof(char));
		if (!t_package->str)
			exit_with_error("Error: memory allocation failed");
		t_package->str[t_package->data] = '\0';
		t_package->bits = 0;
	}
}

static void	s_handler(int num, siginfo_t *info, void *context)
{
	static t_protocol	t_package;
	static int			i;

	usleep(100);
	(void)context;
	decode_signal(&t_package, num);
	init_msg_storage(&t_package);
	parsing_message(&t_package, &i, info->si_pid);
	ft_ping(info->si_pid, 0, 0);
}

int	main(void)
{
	struct sigaction	s_s;

	sigemptyset(&s_s.sa_mask);
	s_s.sa_sigaction = s_handler;
	s_s.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction_configure(&s_s);
	ft_printf("server [PID = %d]\n", getpid());
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
