CC=mpicc
SOURCE_FOLDER=src
BUILD_FOLDER=bin
EXE=$(BUILD_FOLDER)/mandelbot

FLAGS_WARNINGS=-W -Wall
FLAGS_MODE=-std=c99
FLAGS_PERFORMANCE=-Ofast -ffast-math -march=core2
FLAGS=$(FLAGS_WARNINGS) $(FLAGS_MODE) $(FLAGS_PERFORMANCE)

all: main

dirs:
	mkdir $(BUILD_FOLDER)
	mkdir output

main:
	$(CC) $(FLAGS) -o $(EXE) $(SOURCE_FOLDER)/*.c

clean:
	rm -f $(BUILD_FOLDER)/*
	rm -f output/*
	rm -f Mandelbrot.e*
	rm -f Mandelbrot.o*
	rm -f machine.file.*
