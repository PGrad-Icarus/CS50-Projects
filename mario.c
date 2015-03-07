#include <stdio.h>
#include <cs50.h>
int main(void) 
{
    int height = -1;
    
    while (height > 23 || height < 0)  
    {
        printf("Please input a non-negative number less than or equal to 23:\n");
        height = GetInt();
    }
    
    for (int line = 1; line <= height; line++)
    {
        //spaces
        for (int space = height - line; space > 0; space--)
        {
            printf(" ");
        }
        //hashes
        for (int hash = 0; hash < (line + 1) ; hash++)
        {
            printf("#");  
        }
        printf("\n");
    }
    return 0;
} 
