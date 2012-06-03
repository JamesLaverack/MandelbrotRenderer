#include "mandelbot.h"

int main(int argv, char** args)
{
    // Major variables
    int width, height, max_iteration;
    float fwidth, fheight, fmax_iteration;
    FILE *file;
    
    // Set variables
    width = 3500;
    height = 2000;
    max_iteration = 1000;
    
    // Grid
    uint8 **grid;
    
    grid = calloc(width, sizeof(uint8*));
    for(int i=0;i<width;i++)
    {
        grid[i] = (uint8*) calloc(height, sizeof(uint8));
    }
    
    // Convert to floats for non-integer division later
    fwidth = width;
    fheight = height;
    fmax_iteration = max_iteration;
    
    // Say hello
    printf("Mandelbot set renderer.\n");
    printf("    Rendering at %d, %d.\n", width, height);
    printf("    Using %d iterations.\n", max_iteration);
    printf("Begin...\n");
    
    // 1% is
    int one_percent = (width*height)/100;
    int pixels_done = 0;
    int last_percentile = 0;
    // Loop for all pixels in image
    for(int j=0;j<width;j++)
    {
      for(int i=0;i<height;i++)
      {
        float x0 = (j/fwidth)*3.5 - 2.5;
        float y0 = -1*((i/fheight)*2 - 1);

        //printf("        test %f, %f\n", x0, y0);
        
        float x = x0;
        float y = y0;
        int iteration = 0;
        float xtemp;
        while ( x*x + y*y < 2*2  && iteration < max_iteration )
        {
          xtemp = x*x - y*y + x0;
          y = 2*x*y + y0;

          x = xtemp;
          //printf("            [%d] Move to %f, %f\n", iteration, x, y);
          iteration++;
        }

        //printf("            get %d iterations.\n", iteration);
        grid[j][i] = (iteration/fmax_iteration)*255;
        // Done a pixel
        pixels_done++;
        if(pixels_done>=(last_percentile+1)*one_percent)
        {
          last_percentile++;
          printf("    %d\n", last_percentile);
        }
      }
      
    }
    
    // Write out image
    printf("Done. Writing file.\n");
    
    file = fopen("output.ppm", "w");
    // Print PPM format header
    fprintf(file,"P3\n%d\n%d\n255\n", width, height);
    for(int i=0;i<height;i++)
    {
      for(int j=0;j<width;j++)
      {
        fprintf(file,"%d %d %d ", grid[j][i], grid[j][i], grid[j][i]);
      }
      fprintf(file,"\n");
    }
    fclose(file);
    
    // Done
    printf("Finished.\n");
    return EXIT_SUCSESS;
}