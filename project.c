

// Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define
#define MAX_WIDTH 1000
#define MAX_HEIGHT 1000

#define FILTER_WIDTH 5
#define FILTER_HEIGHT 5

#define MAX_LENGTH 100

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
    char filename[MAX_LENGTH];
    char dataArray[MAX_WIDTH][MAX_HEIGHT][3];
    char newArray[MAX_HEIGHT][MAX_WIDTH][3];
} fileData;

fileHeader fHeader;
fileInfoHeader fInfoHeader;
fileData fdata;

//global variables
double filter[FILTER_WIDTH][FILTER_HEIGHT] = {1,1,1,1,1,
                                              1,1,1,1,1,
                                              1,1,1,1,1,
                                              1,1,1,1,1,
                                              1,1,1,1,1};
double factor = 25.0;
double bias = 0.0;

// Function Declarations
int readFile();
char interface();
void printPicture();
void colorSeparation(int input[]);
void blur();
void movimiento();
void matrices(double filter[][5], double factor, double bias);
void createFile();
void bordes();
void monocromatico();
void espejo();

// Functions
int main(int agrc, char *agrv[])
{
    setbuf(stdout, NULL);

    int option = atoi(agrv[1]);
    strcpy(fdata.filename, agrv[2]);
    if(readFile(fdata.filename))
    {
        int input[3];
        input[0] = atoi(agrv[3]);
        input[1] = atoi(agrv[4]);
        input[2] = atoi(agrv[5]);

        switch (option) {
            case 1: printf("1\n"); colorSeparation(input); break;
            case 2: printf("2\n"); monocromatico(); break;
            case 3: printf("3\n"); blur(); break;
            case 4: printf("4\n"); espejo(); break;
            case 5: printf("5\n"); bordes(); break;
            case 6: printf("6\n"); movimiento(); break;
        }
    }
    createFile();
}

int readFile()
{
    printf("Nombre de Archivo: %s\n", fdata.filename);

    FILE *file;
    file = fopen(fdata.filename, "rb");

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

            fclose(file);
            return 1;

        } else printf("La imagen no está en formato BMP.\n");
    }
    return 0;
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
                        fdata.newArray[i][j][0] = 0xFF;
                        fdata.newArray[i][j][1] = 0xFF;
                        fdata.newArray[i][j][2] = 0xFF;
                    }
                } else {
                    fdata.newArray[i][j][0] = 0xFF;
                    fdata.newArray[i][j][1] = 0xFF;
                    fdata.newArray[i][j][2] = 0xFF;
                }
            } else {
                fdata.newArray[i][j][0] = 0xFF;
                fdata.newArray[i][j][1] = 0xFF;
                fdata.newArray[i][j][2] = 0xFF;
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

void createFile()
{
    char path[MAX_LENGTH] = "assets/processed/";
    char name[MAX_LENGTH];

    for(int i = 15; i <= strlen(fdata.filename); i++)
    {
        name[i - 15] = fdata.filename[i];
    }
    strcat(path, name);

    printf("%s\n", path);
    FILE *file;
    file = fopen(path,"w");

    fHeader.offset = sizeof(fHeader) + sizeof(fInfoHeader) - 2;

    fwrite(&fHeader.signature, sizeof(fHeader.signature), 1, file);
    fwrite(&fHeader.size, sizeof(fHeader.size), 1, file);
    fwrite(&fHeader.reserved1, sizeof(fHeader.reserved1), 1, file);
    fwrite(&fHeader.reserved2, sizeof(fHeader.reserved2), 1, file);
    fwrite(&fHeader.offset, sizeof(fHeader.offset), 1, file);
    fwrite(&fInfoHeader, sizeof(fInfoHeader), 1, file);
    for(int i = 0; i < fInfoHeader.height; i++)
    {
        for(int j = fInfoHeader.width - 1; j >= 0; j--)
        {
            for(int k = 2; k >= 0; k--)
            {
                fwrite(&fdata.newArray[i][j][k], sizeof(char), 1, file);
            }
        }
    }

    fclose(file);
}

void matrices(double filter[][5], double factor, double bias)
{
    for(int i = 0; i < fInfoHeader.height - 1; i++)
        for(int j = 0; j < fInfoHeader.width - 1; j++)
        {
            double RGB[3] = {0,0,0};

            for(int filX = 0; filX < FILTER_HEIGHT; filX++)
                for(int filY = 0; filY < FILTER_WIDTH; filY++)
                {
                        RGB[0] += fdata.dataArray[i - FILTER_WIDTH / 2 + filX][j - FILTER_HEIGHT / 2 + filY][0] * filter[filX][filY];
                        RGB[1] += fdata.dataArray[i - FILTER_WIDTH / 2 + filX][j - FILTER_HEIGHT / 2 + filY][1] * filter[filX][filY];
                        RGB[2] += fdata.dataArray[i - FILTER_WIDTH / 2 + filX][j - FILTER_HEIGHT / 2 + filY][2] * filter[filX][filY];

                }

            RGB[0] = RGB[0] / factor + bias;
            if(RGB[0] > 255)
                RGB[0] = 255;
            else if(RGB[0] < 0)
                RGB[0] = 0;

            RGB[1] = RGB[1] / factor + bias;
            if(RGB[1] > 255)
                RGB[1] = 255;
            else if(RGB[1] < 0)
               RGB[1] = 0;


            RGB[2] = RGB[2] / factor + bias;
            if(RGB[2] > 255)
                RGB[2] = 255;
            else if(RGB[2] < 0)
                RGB[2] = 0;

            fdata.newArray[i][j][0] = RGB[0];
            fdata.newArray[i][j][1] = RGB[1];
            fdata.newArray[i][j][2] = RGB[2];
        }
}

void blur()
{
    double filter[FILTER_WIDTH][FILTER_HEIGHT] = {  1,1,1,1,1,
                                                    1,1,1,1,1,
                                                    1,1,1,1,1,
                                                    1,1,1,1,1,
                                                    1,1,1,1,1};
    double factor = 25.0;
    double bias = 0.0;

    matrices(filter, factor, bias);
}

void movimiento()
{
    double filter2[FILTER_WIDTH][FILTER_HEIGHT] = { 1,0,0,0,0,
                                                    0,1,0,0,0,
                                                    0,0,1,0,0,
                                                    0,0,0,1,0,
                                                    0,0,0,0,1};
    double factor2 = 5.0;
    double bias2 = 0.0;

    matrices(filter2, factor2, bias2);
}

void bordes()
{
    double filter[FILTER_WIDTH][FILTER_HEIGHT] = {  0,0,-1,0,0,
                                                    0,0,-1,0,0,
                                                    -1,-1,8,-1,-1,
                                                    0,0,-1,0,0,
                                                    0,0,-1,0,0};
    double factor = 1.0;
    double bias = 0.0;

    matrices(filter, factor, bias);
}

void monocromatico()
{
    int avg;

    for(int i = 0; i < fInfoHeader.height - 1; i++)
        for(int j = 0; j < fInfoHeader.width - 1; j++)
        {
            for(int k = 0; k < 3; k++)
                avg += fdata.dataArray[i][j][k];
            avg = avg / 3;

            for(int k = 0; k < 3; k++)
                fdata.newArray[i][j][k] = avg;
            avg = 0;
        }
}

void espejo()
{
    int aux;

     for(int i = 0; i < fInfoHeader.height - 1; i++)
        for(int j = fInfoHeader.width - 1; j > 0; j--)
            for(int k = 0; k < 3; k++)
            {
                aux = fdata.dataArray[i][j][k];
                fdata.newArray[i][fInfoHeader.width - j][k] = aux;
            }
}
