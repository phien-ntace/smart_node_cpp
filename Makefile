# Target name
TARGET = smart_node

# Compiler
CXX = g++

# List of source file
SRCS = main.cpp hal/led_controller.cpp hal/bh1750.cpp

# List of object file
OBJS = $(SRCS:.cpp=.o)

# Rule compile
all: $(TARGET)

# Link object files to target file
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

# Compile object files
%.o: %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean


# # Makefile for only one main.cpp
# # Target name
# TARGET = smart_node

# # Compiler
# CXX = g++

# # List of source code
# SRCS = main.cpp

# # Rule compile code: g+ main.cpp -o smart_node
# all: $(TARGET)
# $(TARGET):$(SRCS)
# 	$(CXX) $(SRCS) -o $@

# clean:
# 	rm -f $(TARGET)

# .PHONY: all clean
