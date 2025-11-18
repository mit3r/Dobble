# Makefile for Dobble project

CXX = g++ # Compiler
CXXFLAGS = -Wall # Compiler flags 

SERVER_CXXFLAGS = $(CXXFLAGS) # Server compiler flags
CLIENT_CXXFLAGS = $(CXXFLAGS) -I/usr/include/SFML # Client compiler flags

CLIENT_LINKERFLAGS = -lsfml-graphics -lsfml-window -lsfml-system # Linker flags

SERVER_SRC = $(shell find src/server -type f -name '*.cpp') # Server source file
CLIENT_SRC = $(shell find src/client -type f -name '*.cpp') # Client source files

SERVEROBJ = $(SERVER_SRC:.cpp=.o) # Server object files
CLIENTOBJ = $(CLIENT_SRC:.cpp=.o) # Client object files

SERVEREXEC = dobble_server # Executable name
CLIENTEXEC = dobble_client # Executable name

# Pattern rule: compile any .cpp to its corresponding .o
%.o: %.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the server executable
$(SERVEREXEC): $(SERVEROBJ)
	$(CXX) -o build/$@ $^

# Build the client executable
$(CLIENTEXEC): $(CLIENTOBJ)
	$(CXX) -o build/$@ $^ $(CLIENT_LINKERFLAGS)

# Test script
TESTSRC = scripts/runner.cpp
TESTOBJ = $(TESTSRC:.cpp=.o)
TESTEXEC = ${TESTSRC:.cpp=}

# Compile test source files
$(TESTOBJ): $(TESTSRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the test executable
$(TESTEXEC): $(TESTOBJ)
	$(CXX) -o $@ $^

# Build
all: server client

server: $(SERVEREXEC) 
	make clean

client: $(CLIENTEXEC)
	make clean

# Test modes
test1n1: all $(TESTEXEC) clean_test
	./$(TESTEXEC) "Server" "./build/$(SERVEREXEC)" "Client" "./build/$(CLIENTEXEC)"

test1n2: all $(TESTEXEC) clean_test
	./$(TESTEXEC) "Server1" "./build/$(SERVEREXEC)" "Client1" "./build/$(CLIENTEXEC)" "Client2" "./build/$(CLIENTEXEC)"

config:
	sudo apt-get install libsfml-dev

# Clean up
clean:
	find src -type f -name '*.o' -delete
	

clean_test:
	rm -f $(TESTOBJ)