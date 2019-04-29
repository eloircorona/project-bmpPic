#include <stdio.h>
#include <string.h>

#define MAX_WIDTH 200
#define MAX_HEIGHT 200

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

fileHeader fHeader;
fileInfoHeader fInfoHeader;

void readFile(char *filename);

int main(char agrc, char *agrv[])
{
    readFile(agrv[1]);
}

void readFile(char filename[])
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

            char dataArray[MAX_WIDTH][MAX_HEIGHT][3];
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

                        dataArray[fInfoHeader.height - i][fInfoHeader.width - j][0] = r;
                        dataArray[fInfoHeader.height - i][fInfoHeader.width - j][1] = g;
                        dataArray[fInfoHeader.height - i][fInfoHeader.width - j][2] = b;
                    }
                }

                for(int i = 0; i < fInfoHeader.height; i++)
                {
                    for(int j = 0; j < fInfoHeader.width; j++)
                    {
                        r = dataArray[fInfoHeader.height - i][fInfoHeader.width - j][0];
                        g = dataArray[fInfoHeader.height - i][fInfoHeader.width - j][1];
                        b = dataArray[fInfoHeader.height - i][fInfoHeader.width - j][2];

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
                            printf("nn");
                        }
                        else if(r == 0xFF && g == 0xFF && b == 0xFF)
                        {
                            printf("..");
                        }
                    }
                    printf("\n");
                }

            }

            fclose(file);

        } else printf("La imagen no está en el formato BMP\n");

    }

}
