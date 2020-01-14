TARGET	:= interpreter

CC		:= g++
C_FLAGS := -std=c++17 -O2 -w

BIN		:= bin
SOURCE	:= source
INCLUDE	:= include

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= ./$(TARGET).exe
else
EXECUTABLE	:= ./$(TARGET)
endif
OBJECTS		:= $(patsubst %.cpp,$(BIN)/%.o,$(notdir $(wildcard $(SOURCE)/*.cpp)))

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@
$(OBJECTS): $(BIN)/%.o: $(SOURCE)/%.cpp
	$(CC) $(C_FLAGS) -I$(INCLUDE) $^ -c -o $@

ifeq ($(OS),Windows_NT)
clean:
	del $(patsubst $(BIN)/%.o,$(BIN)\\%.o,$(OBJECTS)) \
		$(patsubst ./%.exe,.\\%.exe,$(EXECUTABLE))
else
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
endif
run: $(EXECUTABLE)
	$(EXECUTABLE)
