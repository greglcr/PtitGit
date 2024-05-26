# Compiler and flags
CC = g++
CFLAGS = -Wall -g
LDFLAGS = -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto
SSLFLAGS = -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib

# Directories
SRCDIR = src
BINDIR = .bin

# Source files and object files
SRC = $(wildcard $(SRCDIR)/parser.cpp $(SRCDIR)/cmd/*.cpp $(SRCDIR)/lib/*.cpp)
OBJS = $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Executable name
TARGET = ptitGit

# Default target
all: $(TARGET)

# Ensure bin directory exists
$(shell mkdir -p $(BINDIR))

# Linking the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)  $(SSLFLAGS) 

# Compiling each source file to an object file
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SSLFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(BINDIR) $(TARGET)
	rm -rf test/__pycache__
	rm -rf test/server

# Phony targets
.PHONY: all clean



tests: ptitGit
	@python3 test/run-tests.py
run: all
	@#@rm test/result-test/.ptitgit/ -rf
	@cd test/result-test; ../../ptitGit $(args)
	@# use make run args=init

