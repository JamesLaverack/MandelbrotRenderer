CC=gcc
SOURCE_FOLDER=src
OUTPUT_FOLDER=bin
EXE=$(OUTPUT_FOLDER)/mandelbot

FLAGS_WARNINGS=-W -Wall
FLAGS=$(FLAGS_WARNINGS)

all: main

dirs:
	mkdir $(OUTPUT_FOLDER)

main:
	$(CC) $(FLAGS) -o $(EXE) $(SOURCE_FOLDER)/*.c

clean:
	rm $(OUTPUT_FOLDER)/*