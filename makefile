# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: SPLFLIX

# Tool invocations
SPLFLIX: bin/Main.o bin/Session.o bin/User.o bin/Watchable.o bin/BaseAction.o
	@echo 'Building target: Main'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/splflix bin/Main.o bin/Session.o bin/User.o bin/Watchable.o bin/BaseAction.o $(LFLAGS)
	@echo 'Finished building target: Main'
	@echo ' '

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp
bin/BaseAction.o: src/BaseAction.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/BaseAction.o src/BaseAction.cpp
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp
bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

#Clean the build directory
clean: 
	rm -f bin/*
