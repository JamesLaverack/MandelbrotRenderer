CC=gcc
SOURCE_FOLDER=src
OUTPUT_FOLDER=bin
EXE=$(OUTPUT_FOLDER)/mandelbot

FLAGS_WARNINGS=-W -Wall
FLAGS_MODE=-std=c99
FLAGS_PERFORMANCE=-Ofast -ffast-math -march=core2
FLAGS=$(FLAGS_WARNINGS) $(FLAGS_MODE) $(FLAGS_PERFORMANCE)

all: main

dirs:
	mkdir $(OUTPUT_FOLDER)

main:
	$(CC) $(FLAGS) -o $(EXE) $(SOURCE_FOLDER)/*.c

clean:
	rm $(OUTPUT_FOLDER)/*