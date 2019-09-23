// Mykola Maslych for CAP4453 Robot Vision - Fall 2019

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PICSIZE 256

int pic[PICSIZE][PICSIZE];
int outpicx[PICSIZE][PICSIZE];
int outpicy[PICSIZE][PICSIZE];
int maskx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int masky[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
double edges2D[PICSIZE][PICSIZE], maxival;

int main(int argc, char **argv) 
{
    if (argc < 4)
    {
        printf("\nRun this way: \n"
        "$sobel.exe inputName.pgm"
        " (int)lowThresh (int)highThresh\n");
    }
    
    int i, j, p, q, mr, sumx, sumy;
    double lowThreshold, highThreshold;
    FILE *input, *magnFile, *lowOut, *highOut;
    char *foobar;

    argc--; argv++;
    foobar = *argv;
    input = fopen(foobar, "rb");

    argc--; argv++;
    foobar = *argv;
    lowThreshold = atof(foobar);

    argc--; argv++;
    foobar = *argv;
    highThreshold = atof(foobar);
    
    magnFile = fopen("garb34Out.pgm", "wb");
    lowOut = fopen("lowOut.pgm", "wb");
    highOut = fopen("highOut.pgm", "wb");

    fprintf(magnFile,"P5\n");
    fprintf(magnFile, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(magnFile, "255\n");

    fprintf(lowOut,"P5\n");
    fprintf(lowOut, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(lowOut, "255\n");

    fprintf(highOut,"P5\n");
    fprintf(highOut, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(highOut, "255\n");

    // Read in FILE
    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            pic[i][j] = getc(input);
        }
    }

    // Compute two matices of edges
    mr = 1;
    for (i = mr; i < PICSIZE - mr; i++)
    {
        for (j = mr; j < PICSIZE - mr; j++)
        {
            sumx = 0;
            sumy = 0;
            for (p = -mr; p <= mr; p++)
            {
                for (q = -mr; q <= mr; q++)
                {
                    sumx += pic[i + p][j + q] * maskx[p + mr][q + mr];
                    sumy += pic[i + p][j + q] * masky[p + mr][q + mr];
                }
            }
            outpicx[i][j] = sumx;
            outpicy[i][j] = sumy;
        }
    }

    // Combine two matrices of edges
    maxival = 0;
    for (i = mr; i < PICSIZE - mr; i++)
    {
        for (j = mr; j < PICSIZE - mr; j++)
        {
            edges2D[i][j] = sqrt((double)((outpicx[i][j] * outpicx[i][j]) +
                                       (outpicy[i][j] * outpicy[i][j])));

            if (edges2D[i][j] > maxival)
                maxival = edges2D[i][j];
        }
    }

    // Save FILEs out
    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            edges2D[i][j] = (edges2D[i][j] / maxival) * 255;
            int edgeVal = (int)(edges2D[i][j]);
            int lowVal = 0;
            int highVal = 0;
            
            if (edgeVal > lowThreshold)
                lowVal = 255;

            if (edgeVal > highThreshold)
                highVal = 255;

            fprintf(magnFile, "%c", (char)edgeVal);
            fprintf(lowOut, "%c", (char)lowVal);
            fprintf(highOut, "%c", (char)highVal);
        }
    }
}