#include "mandelbrot.h"

void parse_arguments(int argv, char** args, int* width, int* height, int* max_iteration)
{
  int have_set_width = FALSE;
  int have_set_height = FALSE;
  //printf("I have %d arguments.\n",argv);
  if(argv>1)
  {
    // We have arguments
    for(int i=1;i<argv-1;i++)
    {
      if((!strcmp(args[i],"-w"))||(!strcmp(args[i],"--width")))
      {
        i++;
        *width = atoi(args[i]);
        if(!have_set_height)
        {
          // Scale height to be sensible
          *height = *width/3.5*2;
        }
        have_set_width = TRUE;
      }
      else if((!strcmp(args[i],"-h"))||(!strcmp(args[i],"--height")))
      {
        i++;
        *height = atoi(args[i]);
        if(!have_set_width)
        {
          // Scale width to be sensible
          *width = *height/2*3.5;
        }
        have_set_height = TRUE;
      }
      else if((!strcmp(args[i],"-i"))||(!strcmp(args[i],"--max-iterations")))
      {
        i++;
        *max_iteration = atoi(args[i]);
      }
    }
  }
}

int main(int argv, char** args)
{
    // Major variables
    int width, height, max_iteration;
    float fwidth, fheight, fmax_iteration;
    struct timeval  tic, toc; 
    uint8 **gather_grid = NULL;
    uint8 **grid;
    FILE *file;
    
    // OpenMPI variables
    int rank;
    int size;
    int strip_height;
    int strip_start;
    
    // Set variable defaults
    width = 3500;
    height = 2000;
    max_iteration = 1000;
    
    // Set variables from cmdLine
    parse_arguments(argv, args, &width, &height, &max_iteration);
    
    // Begin OpenMPI
    MPI_Init( &argv, &args );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    strip_height = height/size;
    strip_start = strip_height*rank;

    // Allocate grid memory
    grid = calloc(strip_height, sizeof(uint8*));
    for(int i=0;i<strip_height;i++)
    {
        grid[i] = (uint8*) calloc(width, sizeof(uint8));
    }
    
    if(rank==MASTER)
    {
      // make the final array
      gather_grid = calloc(height, sizeof(uint8*));
      for(int i=0;i<height;i++)
      {
          gather_grid[i] = (uint8*) calloc(width, sizeof(uint8));
      }
    }
    
    // Convert to floats for non-integer division later
    fwidth = width;
    fheight = height;
    fmax_iteration = max_iteration;
    
    // Say hello
    if(rank==MASTER)
    {
      printf("Mandelbot set renderer.\n");
      printf("    Rendering at %d, %d.\n", width, height);
      printf("    Using %d iterations.\n", max_iteration);
      printf("    Strip Height: %d.\n", strip_height);
      printf("Begin...\n");
    }
    
    // Debug output
    printf("I am rank %d, I am looking after rows %d to %d\n", rank, strip_start, strip_start+strip_height-1);
    
    // Loop variables
    float x0, y0, x, y, xtemp;
    int iteration;
    
    // Start timing
    gettimeofday(&tic, NULL);
    
    // Loop for all pixels in image
    for(int i=0;i<strip_height;i++)
    {
      for(int j=0;j<width;j++)
      {
        x0 = (j/fwidth)*3.5 - 2.5;
        y0 = -1*(((i+strip_start)/fheight)*2 - 1);

        //printf("        test %f, %f\n", x0, y0);
        
        x = x0;
        y = y0;
        iteration = 0;
        while ( x*x + y*y < 2*2  && iteration < max_iteration )
        {
          xtemp = x*x - y*y + x0;
          y = 2*x*y + y0;

          x = xtemp;
          //printf("            [%d] Move to %f, %f\n", iteration, x, y);
          iteration++;
        }

        //printf("            get %d iterations.\n", iteration);
        grid[i][j] = (iteration/fmax_iteration)*255;
      }
      
    }
    
    // Gather together
    MPI_Gather(grid, width*strip_height, MPI_UNSIGNED_CHAR,
              gather_grid, width*strip_height, MPI_UNSIGNED_CHAR,
              MASTER, MPI_COMM_WORLD);
    
    MPI_Finalize();
    
    // End timing
    gettimeofday(&toc, NULL);
    
    if(rank==MASTER)
    {
      // Announce calculation finish and time
      printf("Done.\n");
      printf("Elapsed time:\t\t%ld (ms)\n",((toc.tv_sec*1000000+toc.tv_usec)-(tic.tv_sec*1000000+tic.tv_usec))/1000);
      printf("Writing file.\n");
      
      // Write out image
      file = fopen("output.ppm", "w");
      
      // Print PPM format header
      fprintf(file,"P3\n%d\n%d\n255\n", width, height);
      for(int i=0;i<height;i++)
      {
        for(int j=0;j<width;j++)
        {
          fprintf(file,"%d %d %d ", gather_grid[i][j], gather_grid[i][j], gather_grid[i][j]);
        }
        fprintf(file,"\n");
      }
      fclose(file);
      // Done
      printf("Finished.\n");
    }
    
    return EXIT_SUCSESS;
}
