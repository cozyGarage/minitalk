# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/11 15:40:27 by thanh-ng          #+#    #+#              #
#    Updated: 2025/10/11 16:12:38 by thanh-ng         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# =============================================================================
#	GENERAL CONFIG & FLAGS
# =============================================================================

# Program for compiling C programs
CC := cc

# Extra flags to give to the C compiler
CFLAGS := -Wall -Wextra -Werror

# Debug mode
DEBUG ?= 0
ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG -g
endif

RM := /bin/rm -f

NAME_SERVER := server
NAME_CLIENT := client

# =============================================================================
#	MANDATORY PART
# =============================================================================

SRC_SERVER := server.c minitalk.c ft_printf.c
SRC_CLIENT := client.c minitalk.c ft_printf.c

SRC_SERVER_BONUS := server_bonus.c minitalk_bonus.c ft_printf.c
SRC_CLIENT_BONUS := client_bonus.c minitalk_bonus.c ft_printf.c

INCLUDES := -I. -Ilibft

LIBFT := ./libft/libft.a
LIBFT_PATH := ./libft

# =============================================================================
#	RULES
# =============================================================================

all: $(NAME_SERVER) $(NAME_CLIENT)

debug: DEBUG=1

debug: fclean all
	@echo "Built with debug flags (-g, -DDEBUG)"

$(NAME_SERVER): $(LIBFT)
	$(CC) $(CFLAGS) $(SRC_SERVER) $(LIBFT) -o $(NAME_SERVER)

$(NAME_CLIENT): $(LIBFT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) $(LIBFT) -o $(NAME_CLIENT)

# Make libft
$(LIBFT): $(shell make -C $(LIBFT_PATH) -q libft.a)
	make -C $(LIBFT_PATH)

# Clean object files (*.o) - not used on this project
clean:

# Clean object files (*.o) and the binary file
fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)

# Clean object files (*.o) and the binary file; 
# Then create the binary file again, and generate the library and index it
re: fclean all

# "To turn in bonuses to your project, you must include a rule bonus to your Makefile"
# "Makefile must not relink"
bonus: $(LIBFT) fclean
	$(CC) $(CFLAGS) $(SRC_SERVER_BONUS) $(LIBFT) -o $(NAME_SERVER)
	$(CC) $(CFLAGS) $(SRC_CLIENT_BONUS) $(LIBFT) -o $(NAME_CLIENT)

rebonus: fclean bonus

# .PHONY rule in order to avoid relinking
.PHONY: all clean fclean re bonus rebonus