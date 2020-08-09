#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give one argument, no more, no less.");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    FILE *img = 0;
    typedef uint8_t BYTE;
    unsigned char *buffer = malloc(512);
    int img_counter = 0;
    bool jpg_found = false;
    char filename[80];

    if (card == NULL)
    {
        printf("The file cannot be opened.");
        return 1;
    }

    fread(buffer, sizeof(BYTE), 512, card);

    while (!feof(card) && !ferror(card))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && !jpg_found)
        {
            sprintf(filename, "%03i.jpg", img_counter);
            img = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), 512, img);
            jpg_found = true;
            img_counter++;
        }
        else if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && jpg_found)
        {
            fclose(img);
            sprintf(filename, "%03i.jpg", img_counter);
            img = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), 512, img);
            img_counter++;
        }
        else if (jpg_found)
        {
            fwrite(buffer, sizeof(BYTE), 512, img);
        }

        fread(buffer, sizeof(BYTE), 512, card);
    }
    fclose(img);
    free(buffer);
}



