MAIN = main
OBJECTS	= $(MAIN).o
FLAGS = -Wall -Wextra -std=c++17 
CC = g++

ifeq ($(OS), Windows_NT)
DEL := del
OUTPUTMAIN := $(MAIN).exe
else
DEL := rm
OUTPUTMAIN := $(MAIN).out
endif

all: main.exe

main.exe: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(MAIN).exe $(FLAGS)

main.o: $(MAIN).cpp
	$(CC) -c $(MAIN).cpp -o $(MAIN).o 

clean:
	$(DEL) $(OBJECTS)
	$(DEL) $(OUTPUTMAIN)
	$(DEL) *.dat
	$(DEL) *.dot

run: all
	./$(OUTPUTMAIN)
	