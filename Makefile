 # Makefile for Dobble project
 SERVER_EXEC := dobble_server
 CLIENT_EXEC := dobble_client

 BUILD_DIR := build

 .PHONY: all server client config clean
 SERVER_BUILD_DIR := $(BUILD_DIR)/server
 CLIENT_BUILD_DIR := $(BUILD_DIR)/client

 SERVER_SRC_DIR := src/server
 CLIENT_SRC_DIR := src/client

 SERVER_SRC := $(shell find $(SERVER_SRC_DIR) -type f -name '*.cpp')
 CLIENT_SRC := $(shell find $(CLIENT_SRC_DIR) -type f -name '*.cpp')

 SERVER_OBJ := $(SERVER_SRC:%.cpp=$(SERVER_BUILD_DIR)/%.o)
 CLIENT_OBJ := $(CLIENT_SRC:%.cpp=$(CLIENT_BUILD_DIR)/%.o)

 SERVER_DEPS := $(SERVER_OBJ:.o=.d)
 CLIENT_DEPS := $(CLIENT_OBJ:.o=.d)

 INC_DIRS := include

 SERVER_INC_DIRS := $(SERVER_SRC_DIR) $(INC_DIRS)
 CLIENT_INC_DIRS := $(CLIENT_SRC_DIR) $(INC_DIRS)

#  SERVER_INC_DIRS := $(INC_DIRS) $(shell find $(SERVER_SRC_DIR) -type d)
#  CLIENT_INC_DIRS := $(INC_DIRS) $(shell find $(CLIENT_SRC_DIR) -type d)

 SERVER_INC_FLAGS := $(addprefix -I,$(SERVER_INC_DIRS))
 CLIENT_INC_FLAGS := $(addprefix -I,$(CLIENT_INC_DIRS))

 CPL := g++

 CPPFLAGS := -ggdb -std=c++17 -Wall -MMD -MP
 SERVER_CPPFLAGS := $(CPPFLAGS) $(SERVER_INC_FLAGS)
 CLIENT_CPPFLAGS := $(CPPFLAGS) $(CLIENT_INC_FLAGS) -I/usr/include/SFML 

 CLIENT_LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

 all: server client

 server: $(BUILD_DIR)/$(SERVER_EXEC)

 client: $(BUILD_DIR)/$(CLIENT_EXEC)

 qserver:
	g++ -std=c++17 -Wall -Isrc/server -Iinclude $(shell find src/server -name '*.cpp') -o build/dobble_server

 qclient:
	g++ -std=c++17 -Wall -Isrc/client -Iinclude $(shell find src/client -name '*.cpp') -o build/dobble_client -lsfml-graphics -lsfml-window -lsfml-system

 $(BUILD_DIR)/$(SERVER_EXEC): $(SERVER_OBJ)
	$(CPL) $(SERVER_OBJ) -o $@

 $(BUILD_DIR)/$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CPL) $(CLIENT_OBJ) -o $@ $(CLIENT_LDFLAGS)

 $(CLIENT_BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CPL) $(CLIENT_CPPFLAGS) -c $< -o $@

 $(SERVER_BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CPL) $(SERVER_CPPFLAGS) -c $< -o $@




 config:
	sudo apt-get install libsfml-dev

 clean:
	rm -rf build/*

 -include $(SERVER_DEPS)
 -include $(CLIENT_DEPS)

 # # Test script
 # TESTSRC = scripts/runner.cpp
 # TESTOBJ = $(TESTSRC:.cpp=.o)
 # TESTEXEC = ${TESTSRC:.cpp=}

 # # Compile test source files
 # $(TESTOBJ): $(TESTSRC)
 # 	$(CPL) $(CPLFLAGS) -c $< -o $@

 # # Build the test executable
 # $(TESTEXEC): $(TESTOBJ)
 # 	$(CPL) -o $@ $^


 # # Test modes
 # test1n1: all $(TESTEXEC) clean_test
 # 	./$(TESTEXEC) "Server" "./build/$(SERVER_EXEC)" "Client" "./build/$(CLIENT_EXEC)"

 # test1n2: all $(TESTEXEC) clean_test
 # 	./$(TESTEXEC) "Server1" "./build/$(SERVER_EXEC)" "Client1" "./build/$(CLIENT_EXEC)" "Client2" "./build/$(CLIENT_EXEC)"

 # clean_test:
 # 	rm -f $(TESTOBJ)
