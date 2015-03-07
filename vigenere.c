#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Please input only two arguments");
        return 1;
    }
    string key = argv[1];
    for (int d = 0, x = strlen(key); d < x; d++)
    {
        if (!isalpha(key[d]))
        {
            printf("Please input an argument with letters only.\n");
            return 1;
        }
    }
    string s = GetString();
    int n = strlen(s);
    int m = strlen(key);
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        char letter = s[i];
        int letVal = (int)letter;
        j = j % m;
        char cipher = key[j];
        int ciphVal = (int)cipher;
        if (ciphVal > 64 && ciphVal < 91)
        {
            ciphVal = ciphVal % 65;
        }
        if (ciphVal > 96 && ciphVal < 123)
        {
            ciphVal = ciphVal % 97;
        }
        if (letVal < 65 || letVal > 122)
        {        
            printf("%c", letVal);
        }
        else
        {
            int result = letVal + ciphVal;
            if (letVal > 96 && letVal < 123 && result > 122)
            {
                result = (result % 122) + 96;
                printf("%c",   result);
            }
            else if (letVal > 64 && letVal < 91 && result > 90)
            {
                result = (result % 90) + 64;
                printf("%c",   result);
            }
            else
            {
                printf("%c", result);
            }
            j++;
        }
        
   }
    printf("\n");
    
    return 0;
}
        
