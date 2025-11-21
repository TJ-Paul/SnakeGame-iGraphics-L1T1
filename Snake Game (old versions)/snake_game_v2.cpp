#include "iGraphics.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define SNAKE_LENGTH 100 // max length of snake
#define RADIUS 10


int x, y; // to find mouse click position

int snakeX[SNAKE_LENGTH], snakeY[SNAKE_LENGTH];
int snake_size = 5;
int direction = 4; // Initial snake movment direction (4 = left)

int foodX = 450, foodY = 450;

bool start = false;
// bool pause = false;
bool game_over = false;


// initialize SNAKE
void initialize_snake()
{
    for (int i = 0; i < snake_size; i++)
    {
        snakeX[i] = 400 + 2 * i * RADIUS; // Position the snake segments horizontally
        snakeY[i] = 100;
    }
}

void move_snake()
{
    // Wrap-around logic
    if (snakeX[0] > SCREEN_WIDTH)   snakeX[0] = 0;
    if (snakeY[0] > SCREEN_HEIGHT)  snakeY[0] = 0;
    if (snakeX[0] < 0)              snakeX[0] = SCREEN_WIDTH;
    if (snakeY[0] < 0)              snakeY[0] = SCREEN_HEIGHT;

    // Move each segment to the position of the segment in front of it
    for (int i = snake_size - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    // Move the head of the snake based on the direction
    if (direction == 4) snakeX[0] -= RADIUS * 2;
    if (direction == 2) snakeY[0] -= RADIUS * 2;
    if (direction == 8) snakeY[0] += RADIUS * 2;
    if (direction == 6) snakeX[0] += RADIUS * 2;

    // Check if the snake eats the food
    if ((snakeX[0] - foodX) * (snakeX[0] - foodX) + (snakeY[0] - foodY) * (snakeY[0] - foodY) <= 4 * RADIUS * RADIUS) // distance formula
    {
        snake_size+=4;         // Increase the size of the snake
        foodX = rand() % SCREEN_WIDTH; // Reposition food
        foodY = rand() % SCREEN_HEIGHT; // Reposition food
    }

    // Check if snake collides with its body
    for (int i = 1; i < snake_size; i++) // Start from 1 to avoid checking the head against itself
    {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            game_over = true;
            start = false;
            break;
        }
    }
}

void iDraw()
{
    iClear();

    if(!start)
    {
        iSetColor(150,200,150);
        iFilledRectangle(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
        iSetColor(0,0,0);
        iText(SCREEN_WIDTH/2-45,SCREEN_HEIGHT/2,"START",GLUT_BITMAP_TIMES_ROMAN_24);
    }
    // if(pause)
    // {
    //     start = false;
    //     iSetColor(150,200,150);
    //     iFilledRectangle(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    //     iSetColor(0,0,0);
    //     iText(SCREEN_WIDTH/2-45,SCREEN_HEIGHT/2,"RESUME?",GLUT_BITMAP_TIMES_ROMAN_24);
    //     iText(SCREEN_WIDTH/2-20,SCREEN_HEIGHT/2-20,"press 'P'",GLUT_BITMAP_TIMES_ROMAN_10);
    // }

    // Draw the snake
    if (start)
    {
        for (int i = 0; i < snake_size; i++)
        {
            iSetColor(255, 255, 255);                    // Set color to white
            iFilledCircle(snakeX[i], snakeY[i], RADIUS); // Draw the snake's segments
            iSetColor(138, 43, 226);
            iFilledCircle(snakeX[0], snakeY[0], RADIUS); // Draw the snake's segments
        }

        // Draw the food
        iSetColor(255, 255, 0); // Set color to yellow
        iFilledCircle(foodX, foodY, 10);
    }
    if (game_over)
    {
        iSetColor(0, 0, 0); // Red color for game over message
        iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        iSetColor(255, 0, 0); // Red color for game over message
        iText(SCREEN_WIDTH/2-80, SCREEN_HEIGHT/2, "Game Over!", GLUT_BITMAP_TIMES_ROMAN_24);
        return;
    }
}

void iMouseMove(int mx, int my) {}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x = mx;
        y = my;
        printf("(x,y) = %d, %d\n", mx, my);
    }
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(mx >= 340 && mx <= 440 && my >= 390 && my <= 430)
        {
            start = true;
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (key == 'w' && direction != 2) direction = 8; // Move up
    if (key == 'a' && direction != 6) direction = 4; // Move left
    if (key == 's' && direction != 8) direction = 2; // Move down
    if (key == 'd' && direction != 4) direction = 6; // Move right


    if (key == 'p') // pause menu
    {
        start = !start;
        game_over = false;
    }
    // if (key == 'o') // pause menu
    // {
    //     start = true;
    // }
}

void iSpecialKeyboard(unsigned char key)
{}

int main()
{
    initialize_snake();
    iSetTimer(75, move_snake); // Move the snake every 100ms
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    return 0;
}
