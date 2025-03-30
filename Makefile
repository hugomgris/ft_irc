# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 13:48:05 by hmunoz-g          #+#    #+#              #
#    Updated: 2025/03/26 13:57:52 by nponchon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -=-=-=-=-    COLOURS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

DEF_COLOR   = \033[0;39m
YELLOW      = \033[0;93m
CYAN        = \033[0;96m
GREEN       = \033[0;92m
BLUE        = \033[0;94m
RED         = \033[0;91m

# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME        := ./ircserv

# -=-=-=-=-    FLAG -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

CPP          = c++
FLAGS       = -Werror -Wall -Wextra -std=c++98 -pedantic -g #-fsanitize=address
DEPFLAGS    = -MMD -MP

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

RM          = rm -fr
OBJ_DIR     = .obj
DEP_DIR     = .dep

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC         := src/main.cpp \
				src/ServerManager.cpp \
				src/SocketManager.cpp \
				src/ClientManager.cpp \
				src/ChannelManager.cpp \
				src/CommandManager.cpp \
				src/objects/ClientBuffer.cpp \
				src/objects/RegisteredClient.cpp \
				src/objects/Channel.cpp \
				src/commands/PassCommand.cpp \
				src/commands/NickCommand.cpp \
				src/commands/UserCommand.cpp \
				src/commands/CapCommand.cpp \
				src/commands/PopulationCommand.cpp \
				src/commands/ChannelListCommand.cpp \
				src/commands/ChannelModesCommand.cpp \
				src/commands/QuitCommand.cpp \
				src/commands/JoinCommand.cpp \
				src/commands/PingCommand.cpp \
				src/commands/PongCommand.cpp \
				src/commands/PrivmsgCommand.cpp \
				src/commands/NoticeCommand.cpp \
				src/commands/ModeCommand.cpp \
				src/commands/TopicCommand.cpp \
				src/commands/WhoCommand.cpp \
				src/commands/InviteCommand.cpp \
				src/commands/KickCommand.cpp \

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS        = $(addprefix $(DEP_DIR)/, $(SRC:.cpp=.d))

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: directories $(NAME)

directories:
	@mkdir -p $(OBJ_DIR)/src/commands
	@mkdir -p $(DEP_DIR)/src/commands
	@mkdir -p $(OBJ_DIR)/src/objects
	@mkdir -p $(DEP_DIR)/src/objects

-include $(DEPS)

$(OBJ_DIR)/%.o: %.cpp 
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
	$(CPP) $(FLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(DEP_DIR)/$*.d

$(NAME): $(OBJS) Makefile
	@echo "$(GREEN)Linking $(NAME)!$(DEF_COLOR)"
	$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"
	@echo "$(RED)Smells like old spirit.$(DEF_COLOR)"

clean:
	@$(RM) $(OBJ_DIR) $(DEP_DIR)
	@echo "$(RED)Cleaned object files and dependencies$(DEF_COLOR)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re directories
