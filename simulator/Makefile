TARGET = simulator

ifeq ($(OS),Windows_NT)
	CXX = g++
	EXE = .exe
	LDFLAGS = -L./lib/win -lraylib -lopengl32 -lgdi32 -lwinmm
	RM = del /Q
	FIX_PATH = $(subst /,\,$1)
	RUN_CMD = .\$(TARGET)$(EXE)
else
	CXX = g++
	EXE =
	LDFLAGS = -L./lib/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	RM = rm -f
	FIX_PATH = $1
	RUN_CMD = ./$(TARGET)$(EXE)
endif

CXXFLAGS = -I./include -Wall -O2 -MMD

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

$(TARGET)$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)$(EXE) $(LDFLAGS)

-include $(DEPS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(TARGET)$(EXE)
	
run: $(TARGET)$(EXE)
	$(RUN_CMD)

.PHONY: clean run
