# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hublanc <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/24 15:04:09 by hublanc           #+#    #+#              #
#    Updated: 2018/10/26 22:52:53 by hublanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Check if HOSTTYPE exist if not give it a value
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

#	Variables
NAME    = libft_malloc_$(HOSTTYPE).so
CC      = gcc
FLAGS   = -Wall -Wextra -Werror -g
LIB     = libft/libft.a
HEADER  = includes/
LIBSRC  = libft/
SRCDIR  = srcs/
OBJDIR	= objs/
SRC		= ft_malloc.c ft_free.c ft_realloc.c show_alloc_mem.c ft_calloc.c\
		  finder.c limit.c block.c area.c
SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(SRC:.c=.o))

#	Colors
GRN     =   \033[0;32m
RED     =   \033[0;31m
CYN     =   \033[0;36m
NC      =   \033[0m
	
all: $(OBJDIR) $(NAME)

$(NAME): $(LIB) $(OBJS)
	$(CC) $(FLAGS) $(LIBSRC)/libft.a -shared -o $(NAME) $(OBJS)
	#$(CC) $(FLAGS) -L./$(LIBSRC) -o Malloc $(OBJS)
	@echo "\n${CYN}PROCESSING DONE !${NC}"

$(OBJDIR):
	@mkdir -p objs

$(LIB):
	@echo "${CYN}Processing ${NC}./libft/objs ${CYN}[${NC}...${CYN}]${NC}"
	@make -C $(LIBSRC)
	@echo "\n${CYN}Processing ${NC}./objs ${CYN}[${NC}...${CYN}]${NC}"

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADER)/ft_malloc.h
	@echo "${GRN}Compiling ${NC} $@"
	$(CC) $(FLAGS) -c -o $@ $< -I $(HEADER)

clean:
	@echo "${RED}Cleaning ${NC}./libft/objs/ ${RED}[${NC}...${RED}]${NC}"
	@rm -rf $(OBJS)

fclean: clean
	@echo "${RED}Cleaning ${NC}./libft/${RED}libft.h${NC}\n"
	@rm -rf $(NAME)
	@echo "${RED}DELETE DONE !${NC}"

re: fclean all

.PHONY: all clean fclean re
