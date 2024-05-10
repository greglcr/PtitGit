# Compiler and flags
CC = g++
CFLAGS = -Wall -g
LDFLAGS = -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto

# Directories
SRCDIR = src
BINDIR = .bin

# Source files and object files
SRC = $(wildcard $(SRCDIR)/parser.cpp $(SRCDIR)/cmd/*.cpp $(SRCDIR)/lib/*.cpp)
OBJS = $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Executable name
TARGET = petitGit

# Default target
all: $(TARGET)

# Ensure bin directory exists
$(shell mkdir -p $(BINDIR))

# Linking the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) 

# Compiling each source file to an object file
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(BINDIR) $(TARGET)

# Phony targets
.PHONY: all clean



tests: petitGit
	@python3 test/run-tests.py
run: all
	@rm test/result-test/.ptitgit/ -rf
	@cd test/result-test; ../../petitGit $(args)
	@# use make run args=init

