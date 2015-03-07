#include <stdio.h>
#include <cs50.h>
#include <math.h>
int main(void) 
{
    float ChangeIn$ = -.01;
    while (ChangeIn$ < 0)
    {
        printf("What's your change?\n");
        ChangeIn$ = GetFloat();
    }
    int ChangeCents = roundf((ChangeIn$ * 100.00));
    int Quarters = ChangeCents / 25;
    int Dimes = (ChangeCents % 25)/10;
    int Nickels = ((ChangeCents % 25) % 10) /5;
    int Pennies = (((ChangeCents % 25) % 10) %5) /1;
    int TotalCoins = Quarters + Dimes + Nickels + Pennies;
    printf("%d\n", TotalCoins);
    
    
}
    
