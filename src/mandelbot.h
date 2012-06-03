#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mpi.h" 

// Data types
#define uint8 unsigned char

// Static Values
#define EXIT_SUCSESS 0
#define FALSE 0
#define TRUE 1
#define MASTER 0

// Function Prototypes
void parse_arguments(int argv, char** args, int* width, int* height, int* max_iteration);