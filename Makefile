RELEASE		:= -O3
DEBUG		:= -ggdb3 -Og

STATIC		:= -Bstatic -static-libgcc -static-libstdc++
SHARED		:= -static-libstdc++

BUILD		:= $(DEBUG)
LINKTYPE	:= $(STATIC)

CXX_FLAGS 	:= -std=c++17 $(BUILD) $(LINKTYPE)
CXX			:= g++

BIN			:= bin
SRC			:= src
INC			:= include
LIB			:= lib
OBJ     	:= obj
RES			:= res

INC_FLAG	:= -I$(INC)
LIB_FLAG	:= -L$(LIB)

ifeq ($(OS),Windows_NT)
	EXECUTABLE		:= olcRelayRace.exe
	LIBRARIES		:= -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -lstdc++fs
	PLATFORM		:= mingw
	CLEAN_COMMAND 	:= del $(BIN)\*.exe $(OBJ)\$(PLATFORM)\*.o
	LINK_FLAGS		:= -static
else
	EXECUTABLE		:= olcRelayRace
	LIBRARIES		:= -lm -lX11 -lGL -lpthread -lpng -lstdc++fs
	PLATFORM		:= linux
	CLEAN_COMMAND 	:= -rm -r $(BIN)/* $(OBJ)/*/*
	LINK_FLAGS		:= 
endif

SOURCES		:= $(wildcard $(SRC)/*.cpp)
OBJECTS		:= $(patsubst $(SRC)/%,$(OBJ)/$(PLATFORM)/%,$(SOURCES:.cpp=.o))

.PHONY: clean all

all: $(BIN)/$(EXECUTABLE)

clean:
	$(CLEAN_COMMAND)

# Compile only
$(OBJ)/$(PLATFORM)%.o : $(SRC)%.cpp $(DEPENDENCIES)
	$(CXX) $(CXX_FLAGS) $(INC_FLAG) -c -o $@ $<

# Link the object files and libraries
$(BIN)/$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $(BIN)/$(EXECUTABLE) $^ $(LIBRARIES) $(LIB_FLAG) $(LINK_FLAGS)