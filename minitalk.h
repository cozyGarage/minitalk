/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:01:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/14 18:44:42 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ======================== DEFINITIONS

#ifndef MINITALK_H
# define MINITALK_H

# define WAIT_US  100 // usleep()
# define CHAR_BIT 8

// ======================== INCLUDES

# include "libft/libft.h" // ft_atoi(), ft_strlen(), ft_putstr(), ft_putnbr()
# include <unistd.h> // getpid(), write()
# include <signal.h> // sigemptyset(), sigaction()
# include <stdlib.h> // exit()

// ======================== MACROS

// Debug macro
#ifdef DEBUG
# define DEBUG_PRINT(...) ft_printf(__VA_ARGS__)
#else
# define DEBUG_PRINT(...)
#endif

// ======================== PROTOTYPES

// Error utility
void exit_with_error(const char *msg);

// Protocol functions
void	sigaction_configure(struct sigaction *sa);
void	send_int(pid_t pid, int num);
void	send_char(pid_t pid, char c);
void	send_bit(pid_t pid, char bit, char flag_to_pause);
void	send_bits(pid_t pid, unsigned long value, int bit_count);

// Printf and atoi
int ft_printf(const char *format, ...); // Added for ft_printf
int ft_atoi(const char *str); // Added for ft_atoi

// ======================== TYPEDEF and STRUCTURES

/*
  Structure to handle the protocol between server-client
  
  bits : bits received
  data : data received, can be an integer (length of message) or char
  flag : 1 if length of message was received, else 0
  message : message received
*/
typedef struct s_protocol
{
	int		bits;
	int		data;
	int		flag;
	char	*message;
}			t_protocol;

#endif