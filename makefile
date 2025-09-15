# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Target
TARGET = simcity

# Directories
OBJDIR = bin

# Detect all .cpp files
SRCS = $(wildcard *.cpp)

# Object files in bin/
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

# Default target
all: $(OBJDIR) $(TARGET)

# Create bin directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link object files into simcity.out
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile .cpp to object files in bin/
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

.PHONY: all clean