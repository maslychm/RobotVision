// Mykola Maslych for CAP4453 Robot Vision - Fall 2019

#include <stdlib.h> /*  Marr-Hildreth.c  (or marrh.c) */
#include <stdio.h>
#include <math.h>
#define PICSIZE 256
#define MAXMASK 100

int pic[PICSIZE][PICSIZE];
int edgeflag[PICSIZE][PICSIZE];
int histogram[PICSIZE];
int final[PICSIZE][PICSIZE];
double maskx[MAXMASK][MAXMASK];
double masky[MAXMASK][MAXMASK];
double convx[PICSIZE][PICSIZE];
double convy[PICSIZE][PICSIZE];
double ival[PICSIZE][PICSIZE];

main(int argc, char **argv)
{
    int i, j, p, q, mr, centx, centy;
    double area, cutOff, percent, slope, sumx;
    double sumy, sig, maxival, xmask, ymask, hi, lo;
    char *foobar;
    FILE *input, *out1, *out2, *out3;

    // $.exe in out1 out2 out3 sig percent

    argc--; argv++;
    foobar = *argv;
    input = fopen(foobar, "rb");

    argc--; argv++;
    foobar = *argv;
    out1 = fopen(foobar, "wb");

    argc--; argv++;
    foobar = *argv;
    out2 = fopen(foobar, "wb");

    argc--; argv++;
    foobar = *argv;
    out3 = fopen(foobar, "wb");

    argc--; argv++;
    foobar = *argv;
    sig = atof(foobar);

    argc--; argv++;
    foobar = *argv;
    percent = atof(foobar);

    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);

    // Read in File
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            pic[i][j] = getc(input);

    // Derivatives for masks
    for (p = -mr; p <= mr; p++)
    {
        for (q = -mr; q <= mr; q++)
        {
            xmask = (q * exp(-1 * ((p * p) + (q * q)) / (2 * (sig * sig))));
            ymask = (p * exp(-1 * ((p * p) + (q * q)) / (2 * (sig * sig))));

            // xmask = q * (exp((-1 * (q * q) / (2 * (sig * sig)))));
            // ymask = p * (exp((-1 * (p * p) / (2 * (sig * sig)))));

            maskx[p + centy][q + centx] = xmask;
            masky[p + centy][q + centx] = ymask;
        }
    }

    // Convolutions in two directions
    for (i = mr; i <= 255 - mr; i++)
    {
        for (j = mr; j <= 255 - mr; j++)
        {
            sumx = 0;
            sumy = 0;
            for (p = -mr; p <= mr; p++)
            {
                for (q = -mr; q <= mr; q++)
                {
                    sumx += pic[i + p][j + q] * maskx[p + centy][q + centx];
                    sumy += pic[i + p][j + q] * masky[p + centy][q + centx];
                }
            }
            convx[i][j] = sumx;
            convy[i][j] = sumy;
        }
    }

    // Calculate magnitudes from two directions
    maxival = 0;
    for (i = mr; i < PICSIZE - mr; i++)
    {
        for (j = mr; j < PICSIZE - mr; j++)
        {
            ival[i][j] = sqrt((double)((convx[i][j] * convx[i][j]) +
                                       (convy[i][j] * convy[i][j])));

            if (ival[i][j] > maxival)
            {
                maxival = ival[i][j];
            }
        }
    }

    // Save Smoothened magnitudes as output for part 1
    fprintf(out1, "P5\n");
    fprintf(out1, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(out1, "255\n");

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(out1, "%c", (char)(int)ival[i][j]);
        }
    }


    // Identify candidates for edges
    // Look in four directions as described in class notes
    for (i = mr; i < PICSIZE - mr; i++)
    {
        for (j = mr; j < PICSIZE - mr; j++)
        {
            if ((convx[i][j]) == 0.0)
            {
                convx[i][j] = .00001;
            }

            slope = convy[i][j] / convx[i][j];
            
            if ((slope <= .4142) && (slope > -.4142))
            {
                if ((ival[i][j] > ival[i][j - 1]) && (ival[i][j] > ival[i][j + 1]))
                {
                    edgeflag[i][j] = 255;
                }
            }
            else if ((slope <= 2.4142) && (slope > .4142))
            {
                if ((ival[i][j] > ival[i - 1][j - 1]) && (ival[i][j] > ival[i + 1][j + 1]))
                {
                    edgeflag[i][j] = 255;
                }
            }
            else if ((slope <= -.4142) && (slope > -2.4142))
            {
                if ((ival[i][j] > ival[i + 1][j - 1]) && (ival[i][j] > ival[i - 1][j + 1]))
                {
                    edgeflag[i][j] = 255;
                }
            }
            else
            {
                if ((ival[i][j] > ival[i - 1][j]) && (ival[i][j] > ival[i + 1][j]))
                {
                    edgeflag[i][j] = 255;
                }
            }
        }
    }

    // Save edge candidates into file
    fprintf(out2, "P5\n");
    fprintf(out2, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(out2, "255\n");

    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            fprintf(out2, "%c", (char)(int)edgeflag[i][j]);


    // PART 3. Automatically pick thresholds
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            histogram[(int)ival[i][j]]++;

    // Find the pixel for a maxval (from slides)
    cutOff = percent / 100.0f * PICSIZE * PICSIZE ;
    area = 0;
    for (i = PICSIZE - 1; i >= 0; i--)
    {
        area += histogram[i];
        if (area > cutOff)
            break;
    }
    
    // Pick HI and LO (from slides)
    hi = i;
    lo = .35 * hi;

    printf("HI Threshold: %f\n", hi);
    printf("LO Threshold: %f\n", lo);

    // Apply thresholds
    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            final[i][j] = 0;
            if (edgeflag[i][j] == 255)
            {   
                if (ival[i][j] > hi)
                {
                    final[i][j] = 255;
                    edgeflag[i][j] = 0;
                }
                else if (ival[i][j] < lo)
                {
                    edgeflag[i][j] = final[i][j] = 0;
                }
            }
        }
    }

    // Leave all > HI
    // Remove all < LO
    // Between LO and HI leave if neghbor edge
    int moretodo = 1;
    while (moretodo == 1)
    {
        moretodo = 0;
        for (i = 0; i < PICSIZE; i++)
        {
            for (j = 0; j < PICSIZE; j++)
            {
                if (edgeflag[i][j] == 255)
                {
                    for (p = -1; p <= 1; p++)
                    {
                        for (q = -1; q <= 1; q++)
                        {
                            if (final[i + p][j + q] == 255)
                            {
                                final[i][j] = 255;
                                edgeflag[i][j] = 0;
                                moretodo = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    // Save final file
    fprintf(out3, "P5\n");
    fprintf(out3, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(out3, "255\n");

    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            fprintf(out3, "%c", (char)((int)(final[i][j])));
}