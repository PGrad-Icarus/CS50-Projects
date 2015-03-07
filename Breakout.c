//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
GObject detectCollision1(GWindow window, GRect laser);
void removeGWindow(GWindow window, GObject object);
void assignVelComps(double vel);
GLabel initNotice(GWindow window, GLabel label, string s, int shiftX, int shiftY);
void controlNotice(int points);
double velx;
double vely;
GWindow window;
GLabel label;
int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);
    
    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    label = initScoreboard(window);
    
    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // keep playing until game over
    double vel = .225;
    
    assignVelComps(vel);
    
    pause(1000);
    
    while (lives > 0 && bricks > 0)
    {
        // TODO
        
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        move(ball, velx, vely);
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velx = -velx;
        }
        if (getX(ball) <= 0)
        {
            velx = -velx;
        }
        
        if (getY(ball) >= getHeight(window))
        {
            if (event != NULL)
            {
                if (getEventType(event) == MOUSE_CLICKED)
                {     
                    GOval ball2 = initBall(window);
                    ball = ball2;
                    lives--;
                    if (lives == 0)
                    {
                        string s = "You lost. What a shame. :(\n";
                        GLabel notice = initNotice(window, label, s, 110, 50);
                    }
                }
            }
        }
        if (getY(ball) <= 0)
        {
            vely = -vely;
        }
        
        GObject object = detectCollision(window, ball);
        
        if (strcmp(getType(object), "GRect") == 0)
        {
            // TODO
            
            vely = -vely;
            
            if (object != paddle)
            {
                removeGWindow(window, object);
                points++;
                updateScoreboard(window, label, points);
                bricks--;
                vely = vely + .0150;
                controlNotice(points);
            }
        }
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, 520);
            }
        }         
    }
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void assignVelComps(double vel)
{
    double signDice = drand48();
    double angle = drand48() * 1.31;
    while (angle < .916 || angle > 1.31)
    {
        angle = drand48() * 1.31;
    }
    velx = vel * cos(angle);
    vely = vel * sin(angle);
    if (signDice < .5)
    {
        velx = -velx;
    }
}
void initBricks(GWindow window)
{
    // TODO
    int brickY = 44;
    GRect brick;
    char* color[5];
    color[0] = "RED";
    color[1] = "ORANGE";
    color[2] = "YELLOW";
    color[3] = "GREEN";
    color[4] = "BLACK";
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            brick = newGRect(2 + (40 * c), brickY, 36, 10);
            setColor(brick, color[r]);
            setFilled(brick, true);
            add(window, brick);
        }
        brickY += 14;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    GOval ball = newGOval(190, 290, 24, 24);
    setColor(ball, "BLUE");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO
    GRect paddle = newGRect(160, 519, 80, 12);
    setColor(paddle, "BLUE");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);
    int points = 0;
    updateScoreboard(window, label, points);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);
    
    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);
    
    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }
    return NULL;
}

void controlNotice(int points)
{
    if (points == 10)
    {
        string s = "Doing good.\n";
        GLabel notice = initNotice(window, label, s, 40, 50);
        pause(1000);
        removeGWindow(window, notice);
    }
    if (points == 25)
    {
        string s = "Halfway there.\n";
        GLabel notice = initNotice(window, label, s, 50, 50);
        pause(1000);
        removeGWindow(window, notice);
    }
    if (points == 40)
    {
        string s = "Almost there!\n";
        GLabel notice = initNotice(window, label, s, 45, 50);
        pause(1000);
        removeGWindow(window, notice);
    }
    if (points == 50)
    {
        string s = "You won! :)\n";
        GLabel notice = initNotice(window, label, s, 30, 50);
    }
}
GLabel initNotice(GWindow window, GLabel label, string s, int shiftX, int shiftY)
{
    GLabel notice = newGLabel("");
    setFont(notice, "SansSerif-20");
    add(window, notice);
    setLabel(notice, s);
    double xn = ((getWidth(window) - getWidth(label)) / 2) - shiftX;
    double yn = ((getHeight(window) - getHeight(label)) / 2) - shiftY;
    setLocation(notice, xn, yn);
    return notice;
}
