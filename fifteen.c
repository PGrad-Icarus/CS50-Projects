/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;
int k;
int l;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    k = d - 1;
    l = k;
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won() == true)
        {
            printf("ftw!\n");
            return 0;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (move(tile) == false)
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // TODO
    for (int r = 0; r < d; r++)
    {
        for (int c = 0; c < d;c++)
        {
            board[r][c] = (d * (d - r)) - (c + 1);
        }
    }
    if (d % 2 == 0)
    {
        int swap_1_and_2 = board[d - 1][d - 3];
        board[d - 1][d - 3] = board[d - 1][d - 2];
        board[d - 1][d - 2] = swap_1_and_2; 
    }
    
    //test for won
    /*for (int r = 0; r < d; r++)
    {
        for (int c = 0; c < d;c++)
        {
            board[r][c] = (r * d) + (c + 1);
        }
    }*/
}
/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    for (int line = 0; line < d; line++)
    {
        for (int digits = 0; digits < d;digits++)
        {
            if (board[line][digits] < 10 && d > 3)
            {
                if (board[line][digits] == 0)
                {
                    printf(" _ ");
                }
                else
                {
                    printf(" %i ", board[line][digits]);
                }
            }
            else
            {
                
                if (board[line][digits] == 0)
                {
                    printf("_ ");
                }
                else
                {
                    printf("%i ", board[line][digits]);
                }
            }
        }
        printf("\n");
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    
    if (board[k - 1][l] == tile)
    {
        int swap = board[k - 1][l];
        board[k - 1][l] = board[k][l];
        board[k][l] = swap;
        k = k - 1;
        return true;
    }
    else if (tile == board[k + 1][l])
    {
        int swap = board[k + 1][l];
        board[k + 1][l] = board[k][l];
        board[k][l] = swap;
        k = k + 1;
        return true;                
    }
    else if (board[k][l - 1] == tile)
    {
        int swap = board[k][l - 1];
        board[k][l - 1] = board[k][l];
        board[k][l] = swap;
        l = l - 1;
        return true;
    }
    else if (tile == board[k][l + 1])
    {
        int swap = board[k][l + 1];
        board[k][l + 1] = board[k][l];
        board[k][l] = swap;
        l = l + 1;
        return true;                
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    int r = 0;
    int c = 0;
    int check = 1;
    do
    {   
        if (board[r][c] == check)
        {
            c += 1;
            check += 1;
            if (c == d)
            {
                c = 0;
                r += 1;
            }   
        }
        else
        {
            return false;
        }
    }
    while (r != (d - 1) || c != (d - 1));
        
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
