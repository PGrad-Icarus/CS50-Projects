/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
int main(int argc, char* argv[])
{
    // TODO
    typedef uint8_t  BYTE;
    typedef struct
    {
        BYTE id[4];
        BYTE block[508];
    } __attribute__((__packed__))
    BUFFER;
    BUFFER b;

    FILE* file = fopen("card.raw", "r");
    
    int counter = -1;
    fread(&b, sizeof(BYTE), 512, file);
    while ((b.id[0] != 0xff || b.id[1] != 0xd8 || b.id[2] != 0xff) || (b.id[3] != 0xe0 && b.id[3] != 0xe1))
    {
        fread(&b, sizeof(BYTE), 512, file);
    }
    while (counter < 49)
    {
            counter++;
            char title[8];
            if (counter < 10)
            {
                sprintf(title, "00%i.jpg", counter);
            }
            else
            {
                sprintf(title, "0%i.jpg", counter);
            } 
            FILE* img = fopen(title, "w");
            fwrite(&b, sizeof(BYTE), 512, img);
            fread(&b, sizeof(BYTE), 512, file);
            while ((b.id[0] != 0xff || b.id[1] != 0xd8 || b.id[2] != 0xff) || (b.id[3] != 0xe0 && b.id[3] != 0xe1))
            {
                fwrite(&b, sizeof(BYTE), 512, img);
                fread(&b, sizeof(BYTE), 512, file);  
                if ((counter == 49) && (feof(file)))
                {
                    break;   
                }
            }
            fclose(img); 
    }                             
    fclose(file);
    return 0;
}
