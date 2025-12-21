 # Makefile for Dobble project
 LOBBY_SERVER_EXEC := lobby_server
 GAME_SERVER_EXEC := game_server
 CLIENT_EXEC := dobble_client

 BUILD_DIR := build

 .PHONY: all lobby_server game_server client config clean
 LOBBY_BUILD_DIR := $(BUILD_DIR)/obj_lobby
 GAME_BUILD_DIR := $(BUILD_DIR)/obj_game
 CLIENT_BUILD_DIR := $(BUILD_DIR)/obj_client

 LOBBY_SRC_DIR := src/server/lobbyserver
 GAME_SRC_DIR := src/server/gameserver
 COMMON_SERVER_SRC_DIR := src/server
 CLIENT_SRC_DIR := src/client
 PROTOCOL_SRC_DIR := src/protocol

 # Lobby server sources
 LOBBY_SRC := $(shell find $(LOBBY_SRC_DIR) -type f -name '*.cpp')
 LOBBY_SRC += $(COMMON_SERVER_SRC_DIR)/BaseServer.cpp
 LOBBY_SRC += $(COMMON_SERVER_SRC_DIR)/ServerUtils.cpp
 LOBBY_SRC += $(PROTOCOL_SRC_DIR)/utils/SendAndReceiveUtils.cpp

 # Game server sources
 GAME_SRC := $(shell find $(GAME_SRC_DIR) -type f -name '*.cpp')
 GAME_SRC += $(COMMON_SERVER_SRC_DIR)/BaseServer.cpp
 GAME_SRC += $(COMMON_SERVER_SRC_DIR)/ServerUtils.cpp
 GAME_SRC += $(PROTOCOL_SRC_DIR)/utils/SendAndReceiveUtils.cpp

 # Client sources
 CLIENT_SRC := $(shell find $(CLIENT_SRC_DIR) -type f -name '*.cpp')

 LOBBY_OBJ := $(LOBBY_SRC:%.cpp=$(LOBBY_BUILD_DIR)/%.o)
 GAME_OBJ := $(GAME_SRC:%.cpp=$(GAME_BUILD_DIR)/%.o)
 CLIENT_OBJ := $(CLIENT_SRC:%.cpp=$(CLIENT_BUILD_DIR)/%.o)

 LOBBY_DEPS := $(LOBBY_OBJ:.o=.d)
 GAME_DEPS := $(GAME_OBJ:.o=.d)
 CLIENT_DEPS := $(CLIENT_OBJ:.o=.d)

 INC_DIRS := include

 SERVER_INC_FLAGS := -Iinclude
 CLIENT_INC_FLAGS := -Iinclude -I/usr/include/SFML 

 CPL := g++

 CPPFLAGS := -ggdb -std=c++17 -Wall -MMD -MP
 LOBBY_CPPFLAGS := $(CPPFLAGS) $(SERVER_INC_FLAGS)
 GAME_CPPFLAGS := $(CPPFLAGS) $(SERVER_INC_FLAGS)
 CLIENT_CPPFLAGS := $(CPPFLAGS) $(CLIENT_INC_FLAGS)

 CLIENT_LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

 all: lobby_server game_server client

 lobby_server: $(BUILD_DIR)/$(LOBBY_SERVER_EXEC)

 game_server: $(BUILD_DIR)/$(GAME_SERVER_EXEC)

 client: $(BUILD_DIR)/$(CLIENT_EXEC)

 qlobby:
	g++ -std=c++17 -Wall -Iinclude -pthread \
		src/server/lobbyserver/main_server.cpp \
		src/server/lobbyserver/LobbyClientHandler.cpp \
		src/server/lobbyserver/ServerCommandVisitor.cpp \
		src/server/lobbyserver/LobbyServerState.cpp \
		src/server/BaseServer.cpp \
		src/server/ServerUtils.cpp \
		src/protocol/utils/SendAndReceiveUtils.cpp \
		-o build/lobby_server

 qgame:
	g++ -std=c++17 -Wall -Iinclude -pthread \
		src/server/gameserver/game_server.cpp \
		src/server/gameserver/GameClientHandler.cpp \
		src/server/gameserver/ServerCommandVisitor.cpp \
		src/server/BaseServer.cpp \
		src/server/ServerUtils.cpp \
		src/protocol/utils/SendAndReceiveUtils.cpp \
		-o build/game_server

 qclient:
	g++ -std=c++17 -Wall -Iinclude $(shell find src/client -name '*.cpp') -o build/dobble_client -lsfml-graphics -lsfml-window -lsfml-system

 $(BUILD_DIR)/$(LOBBY_SERVER_EXEC): $(LOBBY_OBJ)
	$(CPL) $(LOBBY_OBJ) -o $@ -pthread

 $(BUILD_DIR)/$(GAME_SERVER_EXEC): $(GAME_OBJ)
	$(CPL) $(GAME_OBJ) -o $@ -pthread

 $(BUILD_DIR)/$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CPL) $(CLIENT_OBJ) -o $@ $(CLIENT_LDFLAGS)

 $(CLIENT_BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CPL) $(CLIENT_CPPFLAGS) -c $< -o $@

 $(LOBBY_BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CPL) $(LOBBY_CPPFLAGS) -c $< -o $@

 $(GAME_BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CPL) $(GAME_CPPFLAGS) -c $< -o $@




 config:
	sudo apt-get install libsfml-dev

 clean:
	rm -rf build/*

 -include $(LOBBY_DEPS)
 -include $(GAME_DEPS)
 -include $(CLIENT_DEPS)


