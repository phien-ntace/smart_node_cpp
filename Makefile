# Target name
TARGET = smart_node

# Compiler
CXX = g++

# Linker flags
LDFLAGS = -lgpiod -lpaho-mqttpp3 -lpaho-mqtt3as

# List of source file
SRCS = main.cpp hal/led_controller.cpp hal/bh1750.cpp hal/ili9341.cpp hal/gpio.cpp hal/font.cpp hal/dht11.cpp utility.cpp mqtt/mqtt_client.cpp 

# List of object file
OBJS = $(SRCS:.cpp=.o)

# Rule compile
all: $(TARGET)

# Link object files to target file
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

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
