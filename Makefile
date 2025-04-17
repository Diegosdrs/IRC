# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/14 14:31:28 by dsindres          #+#    #+#              #
#    Updated: 2025/04/17 13:18:46 by dsindres         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     := ircserv

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -Iinclude

SRC_DIR  := src
OBJ_DIR  := obj

SRC      := \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/Client.cpp \
	$(SRC_DIR)/Channel.cpp \
	$(SRC_DIR)/Command.cpp \

OBJ      := \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/Client.o \
	$(OBJ_DIR)/Channel.o \
	$(OBJ_DIR)/Command.o \

# Rules

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "✅ Compilation terminée !"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "🧹 Objets supprimés."

fclean: clean
	@rm -f $(NAME)
	@echo "🧹 Exécutable supprimé."

re: fclean all

.PHONY: all clean fclean re