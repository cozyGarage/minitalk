/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:01:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/14 20:56:18 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

# define CHAR_BIT 8

# include "libft/libft.h" // ft_atoi(), ft_strlen(), ft_putstr(), ft_putnbr()
# include <unistd.h> // getpid(), write()
# include <signal.h> // sigemptyset(), sigaction()
# include <stdlib.h> // exit()
# include <stdbool.h> // bool

void	exit_with_error(const char *msg);
void	sigaction_configure(struct sigaction *sa);
void	ping_int(pid_t pid, int nbr);
void	ping_char(pid_t pid, char c);
void	ft_ping(pid_t pid, char bit, int pause_flag);
int		ft_printf(const char *format, ...); // Added for ft_printf

typedef struct s_protocol
{
	int		bits;
	int		data;
	bool	is_ready;
	char	*str;
}			t_protocol;

#endif
