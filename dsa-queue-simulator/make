# Compiler and flags
CC = g++
CFLAGS = -std=c++14 -Wall -g

# SDL2 configuration
# `sdl2-config --cflags` gets the necessary include paths
# `sdl2-config --libs` gets the necessary library paths
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_ttf

# Additional libraries
LIBS = -lpthread

# Final flags
ALL_CFLAGS = $(CFLAGS) $(SDL_CFLAGS)
ALL_LIBS = $(SDL_LIBS) $(LIBS)

# Source files and object files
SRCS = simulator.cpp window.cpp text.cpp queue.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = window.h text.h queue.h

# Target executable
TARGET = simulator

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(ALL_LIBS)

# Compilation rule for .cpp files
%.o: %.cpp $(HEADERS)
	$(CC) $(ALL_CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Run rule
run: $(TARGET)
	./$(TARGET)

# Phony targets (targets that don't create files with these names)
.PHONY: all clean run