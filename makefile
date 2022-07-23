COMPILER = g++

FLAGS = -Wall --std=c++17

LIBS = -lpthread

MAIN = wakeonlanmanager

BUILD_DIR = build

SRCS = $(wildcard src/*.cpp)

all: $(MAIN)
	$(COMPILER) $(FLAGS) -o $(MAIN) $(SRCS) $(LIBS)
	@echo Project compiled

run: $(MAIN)
	./$(MAIN) $(ARGS)

go: all run