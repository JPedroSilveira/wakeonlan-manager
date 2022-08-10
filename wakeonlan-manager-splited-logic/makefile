PROJECT_NAME = wakeonlanmanager.app

COMPILER = g++

FLAGS = -Wall --std=c++11

LIBS = -pthread

MAIN = src/main.cpp

SRCS = $(shell find ./src -iname "*.cpp")

all: $(PROJECT_NAME)
	@echo Project compiled

$(PROJECT_NAME): $(MAIN)
	$(COMPILER) $(FLAGS) -o $(PROJECT_NAME) $(SRCS) $(LIBS)
	
runclient: $(PROJECT_NAME)
	./$(PROJECT_NAME)

runmanager: $(PROJECT_NAME)
	./$(PROJECT_NAME) manager

clean:
	rm -rf *.o main $(PROJECT_NAME)

manager: clean all runmanager

client: clean all runclient