

// Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define
#define MAX_WIDTH 500
#define MAX_HEIGHT 500

#define FILTER_WIDTH 3
#define FILTER_HEIGHT 3
// Structs
typedef struct fileHeader {
    short signature;
    unsigned int size;
    short reserved1;
    short reserved2;
    unsigned int offset;
} fileHeader;

typedef struct fileInfoHeader {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    short planes;
    short bpp;
    unsigned int compress;
    unsigned int imgsize;
    unsigned int bpmx;
    unsigned int bpmy;
    unsigned int colors;
    unsigned int imxtcolors;
} fileInfoHeader;

typedef struct fileData {
    char dataArray[MAX_WIDTH][MAX_HEIGHT][3];
    char newArray[MAX_HEIGHT][MAX_WIDTH][3];
} fileData;

fileHeader fHeader;
fileInfoHeader fInfoHeader;
fileData fdata;

//global variables
double filter[FILTER_WIDTH][FILTER_HEIGHT] = { 0,0,0,
                                               0,1,0,
                                               0,0,0};
double factor = 0.0;
double bias = 0.0;

// Function Declarations
int readFile(char *filename);
char interface();
void printPicture();
void colorSeparation(int input[]);
void blur();


// Functions
int main(char agrc, char *agrv[])
{
    if(readFile(agrv[1]))
    {
        int input[3];
        input[0] = atoi(agrv[2]);
        input[1] = atoi(agrv[3]);
        input[2] = atoi(agrv[4]);

        colorSeparation(input);
        printPicture();

    } else ;
}

int readFile(char filename[])
{
    printf("Nombre de Archivo: %s\n", filename);

    FILE *file;
    file = fopen(filename, "rb");

    if(!file) printf("No se puede cargar la imagen\n");
    else
    {
        fread(&fHeader.signature, sizeof(fHeader.signature), 1, file);
        fread(&fHeader.size, sizeof(fHeader.size), 1, file);
        fread(&fHeader.reserved1, sizeof(fHeader.reserved1), 1, file);
        fread(&fHeader.reserved2, sizeof(fHeader.reserved2), 1, file);
        fread(&fHeader.offset, sizeof(fHeader.offset), 1, file);

        if(fHeader.signature == 0x4D42)
        {
            fread(&fInfoHeader, sizeof(fInfoHeader), 1, file);

            printf("Header Size: %d\n", fInfoHeader.size);
            printf("Widht: %d\n", fInfoHeader.width);
            printf("Height: %d\n", fInfoHeader.height);
            printf("Planes: %d\n", fInfoHeader.planes);
            printf("Bits per pixel: %d\n", fInfoHeader.bpp);
            printf("Compression: %d\n", fInfoHeader.compress);
            printf("Image Size: %d\n", fInfoHeader.imgsize);

            fseek(file, fHeader.offset, SEEK_SET);

            unsigned char r, g, b;

            if(fInfoHeader.bpp == 24)
            {
               for(int i = fInfoHeader.height - 1 ; i >= 0; i--)
               {
                   for(int j = 0 ; j < fInfoHeader.width ; j++)
                   {
                       fread(&b, sizeof(unsigned char), 1, file);
                       fread(&g, sizeof(unsigned char), 1, file);
                       fread(&r, sizeof(unsigned char), 1, file);

                       fdata.dataArray[fInfoHeader.height - i][fInfoHeader.width - j][0] = r;
                       fdata.dataArray[fInfoHeader.height - i][fInfoHeader.width - j][1] = g;
                       fdata.dataArray[fInfoHeader.height - i][fInfoHeader.width - j][2] = b;
                   }
               }
            }
            return 1;

            fclose(file);

        } else printf("La imagen no está en formato BMP.\n");
        return 0;
    }
}

void colorSeparation(int input[])
{
    char input_r = input[0];
    char input_g = input[1];
    char input_b = input[2];

    for(int i = 0; i < fInfoHeader.height; i++)
    {
        for(int j = 0; j < fInfoHeader.width; j++)
        {
            if(fdata.dataArray[i][j][0] == input_r)
            {
                if(fdata.dataArray[i][j][1] == input_g)
                {
                    if(fdata.dataArray[i][j][2] == input_b)
                    {
                         fdata.newArray[i][j][0] = input_r;
                         fdata.newArray[i][j][1] = input_g;
                         fdata.newArray[i][j][2] = input_b;

                    } else {
                        fdata.newArray[i][j][0] = 0;
                        fdata.newArray[i][j][1] = 0;
                        fdata.newArray[i][j][2] = 0;
                    }
                } else {
                    fdata.newArray[i][j][0] = 0;
                    fdata.newArray[i][j][1] = 0;
                    fdata.newArray[i][j][2] = 0;
                }
            } else {
                fdata.newArray[i][j][0] = 0;
                fdata.newArray[i][j][1] = 0;
                fdata.newArray[i][j][2] = 0;
            }
        }
    }
}

void printPicture()
{
    unsigned char r, g, b;

    for(int i = 0; i < fInfoHeader.height; i++)
    {
        for(int j = 0; j < fInfoHeader.width; j++)
        {
            r = fdata.newArray[fInfoHeader.height - i][fInfoHeader.width - j][0];
            g = fdata.newArray[fInfoHeader.height - i][fInfoHeader.width - j][1];
            b = fdata.newArray[fInfoHeader.height - i][fInfoHeader.width - j][2];

            if(r == 0xFF && g == 0x00 && b == 0x00)
            {
                printf("RR");
            }
            else if(r == 0x00 && g == 0xFF && b == 0x00)
            {
                printf("GG");
            }
            else if(r == 0x00 && g == 0x00 && b == 0xFF)
            {
                printf("BB");
            }
            else if(r == 0x00 && g == 0x00 && b == 0x00)
            {
                printf("..");
            }
            else if(r == 0xFF && g == 0xFF && b == 0xFF)
            {
                printf("..");
            } else {
                printf("UU");
            }
        }
        printf("\n");
    }
}

void blur()
{
    unsigned char r, g, b;

    for(int i = 0; i < fInfoHeader.height; i++)
        for(int j = 0; j < fInfoHeader.width; j++)
        {
            double r = 0.0, g = 0.0, b = 0.0;

            for(int filX = 0; filX < FILTER_HEIGHT; filX++)
                for(int filY = 0; filY < FILTER_WIDTH; filY++)
                {
                    int imgX = (i - FILTER_WIDTH / 2 + filX + fInfoHeader.width) % fInfoHeader.width;
                    int imgY = (j - FILTER_HEIGHT / 2 + filY + fInfoHeader.height) % fInfoHeader.height;
                    r += fdata.dataArray[imgY * fInfoHeader.width][y][0] * filter[filX][filY];
                    g += fdata.dataArray[imgY * fInfoHeader.width][y][1] * filter[filX][filY];
                    b += fdata.dataArray[imgY * fInfoHeader.width][y][2] * filter[filX][filY];
                }

            newArray[][][0] = (factor * r + bias) > 255 ? 255: (factor * r + bias) < 0 ? 0 : (factor * r + bias);
            newArray[][][1] = (factor * g + bias) > 255 ? 255: (factor * g + bias) < 0 ? 0 : (factor * g + bias);
            newArray[][][2] = (factor * b + bias) > 255 ? 255: (factor * b + bias) < 0 ? 0 : (factor * b + bias);
        }   
    

}