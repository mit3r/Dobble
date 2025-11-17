# Makefile for Dobble project

CXX = g++ # Compiler
CXXFLAGS = -c -Wall # Compiler flags 

SFMLCXXFLAGS = -I/usr/include/SFML
SFMLLINKERFLAGS = -lsfml-graphics -lsfml-window -lsfml-system # Linker flags

SERVERSRC = main_server.cpp # Server source file
CLIENTSRC = main_client.cpp # Client source file

SERVEROBJ = $(SERVERSRC:.cpp=.o) # Server object files
CLIENTOBJ = $(CLIENTSRC:.cpp=.o) # Client object files

SERVEREXEC = dobble_server # Executable name
CLIENTEXEC = dobble_client # Executable name

# Build the server executable
$(SERVEREXEC): $(SERVEROBJ)
	$(CXX) -o build/$@ build/$^

# Compile server source files
$(SERVEROBJ): $(SERVERSRC)
	$(CXX) $(CXXFLAGS) $< -o build/$@

# Build the client executable
$(CLIENTEXEC): $(CLIENTOBJ)
	$(CXX) -o build/$@ build/$^ $(SFMLLINKERFLAGS)

# Compile client source files
$(CLIENTOBJ): $(CLIENTSRC)
	$(CXX) $(CXXFLAGS) $(SFMLCXXFLAGS) $< -o build/$@

# Test script
TESTSRC = scripts/runner.cpp
TESTOBJ = $(TESTSRC:.cpp=.o)
TESTEXEC = ${TESTSRC:.cpp=}

# Compile test source files
$(TESTOBJ): $(TESTSRC)
	$(CXX) $(CXXFLAGS) $< -o $@

# Build the test executable
$(TESTEXEC): $(TESTOBJ)
	$(CXX) -o $@ $^

# Build
all: server client
server: $(SERVEREXEC) clean
client: $(CLIENTEXEC) clean

# Test modes
test1n1: all $(TESTEXEC) clean_test
	./$(TESTEXEC) "Server" "./build/$(SERVEREXEC)" "Client" "./build/$(CLIENTEXEC)"

test1n2: all $(TESTEXEC) clean_test
	./$(TESTEXEC) "Server1" "./build/$(SERVEREXEC)" "Client1" "./build/$(CLIENTEXEC)" "Client2" "./build/$(CLIENTEXEC)"

config:
	sudo apt-get install libsfml-dev

# Clean up
clean:
	rm -f build/$(SERVEROBJ) build/$(CLIENTOBJ)

clean_test:
	rm -f $(TESTOBJ)