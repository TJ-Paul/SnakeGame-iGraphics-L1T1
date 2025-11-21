#include "iGraphics.h"
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <string.h>
// #pragma comment(lib, "winmm.lib")
/*
################################
    DISPLAY DETAILS
################################
*/

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700

bool homepage_play = false;
bool homepage_settings = false;
bool homepage_exit = false;
bool pause_button2 = false;
bool gameover_sound = false;
bool homepage_instructions = false;
bool homepage_score = false;
bool back_button = false;
bool music_on = true;
int music_off_count = 1;
int music_on_count = 1;
bool music_tick = true;
bool quick_text2 = true;
bool glowing_ball = false;

/*
################################
    SCOREBOARD DETAILS
################################
*/

#define MAX_NAME_LENGTH 500
#define MAX_HIGH_SCORES 5

char userInput[MAX_NAME_LENGTH];
int inputIndex = 0;
bool inputMode = false;

struct hscore {
    char name[30];
    int score;
} high_score[MAX_HIGH_SCORES];
int len = 0;

bool name_entered = false;
bool show_hsc = false;      // hsc = highcore

/*
################################
    SNAKE DETAILS
################################
*/ 
#define SNAKE_LENGTH 100    // max length of snake
#define RADIUS 10           // food and snake circle radius
int snakeX[SNAKE_LENGTH], snakeY[SNAKE_LENGTH];
int snake_size = 5;         // 
int direction = 4;          // Initial snake direction (4 = left)
bool move = true;
bool left = false;
bool right = false;
/*
################################
    FOOD, SCORE
################################
*/ 
int food_type = 1;              // 1 - small, 2 - big
int small_food_eaten = 0;       // when small_food_eaten == 5, spawn a big food.

// first food_spawn co-ordinates
int foodX = 450, foodY = 450;

int food_radius = RADIUS;
int score = 0;                  // small_food = 1 point. big_food = 2 points
char Score[100];

bool quick_text = true;
clock_t start_time;

int level_2_score = 10;
int level_3_score = 15;

char food_img1[5][100] = {  "project images\\snake food\\food_type_1_1.png",
                            "project images\\snake food\\food_type_1_2.png",
                            "project images\\snake food\\food_type_1_3.png",
                            "project images\\snake food\\food_type_1_4.png",
                            "project images\\snake food\\food_type_1_5.png"};


char food_img2[5][100] = {  "project images\\snake food\\food_type_2_1.png",
                            "project images\\snake food\\food_type_2_2.png",
                            "project images\\snake food\\food_type_2_3.png",
                            "project images\\snake food\\food_type_2_4.png",
                            "project images\\snake food\\food_type_2_5.png"};
int eaten = 1;

bool game_over = false;
int game_state = 0;


/*
################################
    IMAGES USED
################################
*/
char homepage[100]      = "project images\\homepage\\homepage_main.png";
char instructions[100]  = "project images\\instructions_page.png";
char pause_button[100]  = "pause_button.bmp";
char pause_menu[100]    = "project images\\pause_menu.png";


/*
#################################
    INITIALIZE SNAKE
#################################
*/

void initialize_snake()
{
    printf("done snake\n");
    for (int i = 0; i < snake_size; i++)
    {
        snakeX[i] = 400 + 2 * i * RADIUS; // Position the snake segments horizontally
        snakeY[i] = 100;
    }
}

/*
#################################
    SCOREBOARD MANAGEMENT
#################################
*/

void read_high_scores() {
    FILE *fp = fopen("Score.txt", "r");
    if (fp != NULL) {
        len = 0;
        while (fscanf(fp, "%s %d", high_score[len].name, &high_score[len].score) != EOF && len <= MAX_HIGH_SCORES) 
        {
            len++;
        }
        fclose(fp);
    } else {
        printf("Failed to open file.\n");
    }
}

void write_high_scores() {
    FILE *fp = fopen("Score.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < len; i++) {
            fprintf(fp, "%s %d\n", high_score[i].name, high_score[i].score);
        }
        fclose(fp);
    } else {
        printf("Failed to open file.\n");
    }
}

void add_new_score() {
    int pos = -1;
    for (int i = 0; i < len; i++) {
        if (high_score[i].score < score) {
            pos = i;
            break;
        }
    }
    if (pos == -1 && len < MAX_HIGH_SCORES) {
        pos = len;
        len++;
    }

    if (pos != -1) {
        for (int i = len - 1; i > pos; i--) {
            high_score[i] = high_score[i - 1];
        }
        strcpy(high_score[pos].name, userInput);
        high_score[pos].score = score;
    }
}


/*
####################################################
    LEVELS (GAME_STATE)
####################################################

    home menu       game_state = 0;  
    instructions    game_state = 1;  
    pause menu      game_state = 2;
    leaderboard     game_state = 3
    settings        game_state = 4

    level 1         game_state = 11;       
    level 2         game_state = 12; 
    level 3         game_state = 13;
####################################################
*/

void draw_rec()
{
    iSetColor(255,0,0);
    iRectangle(50,50,SCREEN_WIDTH-100, SCREEN_HEIGHT-100);
    iRectangle(52,52,SCREEN_WIDTH-104, SCREEN_HEIGHT-104);
    iShowBMP2(0,0,"project images\\level 2 border.png",0x000000);
}


/*
#############################################
    LEVEL 1
#############################################
*/

void level_1()
{
    // passes through wall
    if (snakeX[0] > SCREEN_WIDTH - RADIUS - 60)     snakeX[0] = RADIUS + 60;
    if (snakeY[0] > SCREEN_HEIGHT - RADIUS - 60)    snakeY[0] = RADIUS + 60;
    if (snakeX[0] < RADIUS + 60)                    snakeX[0] = SCREEN_WIDTH - RADIUS - 60;
    if (snakeY[0] < RADIUS + 60)                    snakeY[0] = SCREEN_HEIGHT - RADIUS - 60;
}


/*
#############################################
    LEVEL 2
#############################################
*/

int positioning = 1;
void level_2()
{   
    // gameover when hits wall
    if(positioning == 1)
    {
        direction = 4;
        snakeX[0] = SCREEN_WIDTH - 300;
        snakeY[0] = SCREEN_HEIGHT - 300;
        positioning ++;
    }
    if (snakeX[0] >= SCREEN_WIDTH - RADIUS - 60)    
    {
        game_over = true;
        gameover_sound = true;
        move = false;
    }
    if (snakeY[0] >= SCREEN_HEIGHT - RADIUS - 60)
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }
    if (snakeX[0] <= RADIUS + 60)  
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }
    if (snakeY[0] <= RADIUS + 60)
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }                
}


/*
#############################################
    LEVEL 3
#############################################
*/

int positioning2 = 1;
void level_3()
{
    // gameover when hits wall
    if(positioning2 == 1)
    {
        direction = 4;
        snakeX[0] = SCREEN_WIDTH - 300;
        snakeY[0] = 100;
        positioning2 ++;
    }
    if (snakeX[0] >= 295 - RADIUS && snakeX[0] <= 360 + RADIUS && snakeY[0] >= 200 - RADIUS && snakeY[0] <= 500 + RADIUS)    
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }
    
    if (snakeX[0] >= 600 - RADIUS && snakeX[0] <= 665 + RADIUS && snakeY[0] >= 200 - RADIUS && snakeY[0] <= 500 + RADIUS)    
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }

    if (snakeX[0] >= SCREEN_WIDTH - RADIUS - 60)    
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }
    if (snakeY[0] >= SCREEN_HEIGHT - RADIUS - 60)
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }
    if (snakeX[0] <= RADIUS + 60)  
    {
        game_over = true;
        gameover_sound = true;
    }
    if (snakeY[0] <= RADIUS + 60)
    {
        game_over = true;
        move = false;
        gameover_sound = true;
    }  
}


/*
########################################################
    FOOD INITIALIZATION
########################################################

    small food      food type = 1; 
    big food        food type = 2;

########################################################
*/
int level_up_sound = 1;
int level_up_sound2 = 1;
void generate_food()
{
    if(small_food_eaten >= 5)
    {
        food_type = 2;
        food_radius = RADIUS * 2;
    }

    if ((snakeX[0] - foodX) * (snakeX[0] - foodX) + (snakeY[0] - foodY) * (snakeY[0] - foodY) <= 4 * food_radius * food_radius) // distance formula
    {
        srand(time(0));
        eaten = rand()%4;

        if(food_type == 1)
        {
            score++; 
            small_food_eaten++; 
            snake_size++;
            PlaySoundA("sound effects\\food eating sound 2.wav", NULL, SND_ASYNC);
        }

        else
        {
            score += 5;
            snake_size += 5;
            small_food_eaten = 0;               // Increase the size of the snake
            food_radius = RADIUS;
            food_type = 1; 
            
            PlaySoundA("sound effects\\big food eating sound.wav", NULL, SND_ASYNC);
            
        }

        // food positioning within the box

        int fx, fy;
        fx = rand() % (SCREEN_WIDTH - 70);      // Reposition food
        fy = rand() % (SCREEN_HEIGHT - 70);     // Reposition food
        if(fx <= 70) foodX = fx + 70;
        else foodX = fx;
        if(fy <= 70) foodY = fy + 70;
        else foodY = fy;
        printf("fx, fy = %d, %d\nfoodX, foodY = %d, %d\n",fx,fy);
        if (fx >= 290 && fx <= 365 && fy >= 195 && fy <= 555)    
        {
            fx = rand() % (SCREEN_WIDTH - 70);      // Reposition food
            fy = rand() % (SCREEN_HEIGHT - 70);
            if(fx <= 70) foodX = fx + 70;
            else foodX = fx;
            if(fy <= 70) foodY = fy + 70;
            else foodY = fy;     // Reposition food
        }
        if (fx >= 595 && fx <= 670 && fy >= 195 && fy <= 555)    
        {
            fx = rand() % (SCREEN_WIDTH - 70);      // Reposition food
            fy = rand() % (SCREEN_HEIGHT - 70);     // Reposition food
            if(fx <= 70) foodX = fx + 70;
            else foodX = fx;
            if(fy <= 70) foodY = fy + 70;
            else foodY = fy;     // Reposition food
        }


        if(score >= level_2_score) 
        {
            game_state = 12;
            if(level_up_sound == 1)
            {
                PlaySoundA("sound effects\\level up sound.wav", NULL, SND_ASYNC);
                level_up_sound++;
            }
        }

        if(score >= level_3_score) 
        {
            game_state = 13;
            if(level_up_sound2 == 1)
            {
                PlaySoundA("sound effects\\level up sound.wav", NULL, SND_ASYNC);
                level_up_sound2++;
            }
        }
    }
}
void move_snake()
{
    if(move)
    {
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
    }

    generate_food();

   // Check snake collission with body
    for (int i = 1; i < snake_size; i++) 
    {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            game_over = true;
            gameover_sound = true;
            break;
        }
    }
}

void draw_obstacle()
{
    iSetColor(255,255,255);
    iRectangle(295,200,65,300);
    iRectangle(600,200,65,300);
}


void draw_snake()
{
    for (int i = 0; i < snake_size; i++)
        {
            iSetColor(255, 255, 255);                    // Set color to white
            iFilledCircle(snakeX[i], snakeY[i], RADIUS); // Draw the snake's segments
            iSetColor(255, 255, 0);
            iFilledCircle(snakeX[0], snakeY[0], RADIUS); // snake's head

            if(direction == 4)  // LEFT
                iShowBMP2(snakeX[0]-17, snakeY[0]-17, "project images\\snake_head\\snake_head_left.png",0x000000);

            if(direction == 6)  // RIGHT
                iShowBMP2(snakeX[0]-17, snakeY[0]-17, "project images\\snake_head\\snake_head_right.png",0x000000);

            if(direction == 8)  // UP
            {
                if(right)
                iShowBMP2(snakeX[0]-17, snakeY[0]-17, "project images\\snake_head\\snake_head_up_right.png",0x000000);
                else 
                iShowBMP2(snakeX[0]-17, snakeY[0]-17, "project images\\snake_head\\snake_head_up_left.png",0x000000);
            }    
                

            if(direction == 2)  // DOWN
            {
                if(right)
                iShowBMP2(snakeX[0]-17, snakeY[0]-17, "project images\\snake_head\\snake_head_down_right.png",0x000000);
                else
                iShowBMP2(snakeX[0]-17, snakeY[0]-17, "project images\\snake_head\\snake_head_down_left.png",0x000000);
            }
        }
}

void level_2_popup()
{
    iShowBMP(950,10,"project images\\levels\\level 2.png");
    if(quick_text)
    {
        if (start_time == 0)
        {
            start_time = clock(); // Set the start time when quick_text is first true
        }
        iText(425,390,"LEVEL 2\n\n",GLUT_BITMAP_TIMES_ROMAN_24);
        if ((clock() - start_time) / CLOCKS_PER_SEC >= 1)
        {
            quick_text = false;
            start_time = 0;
        }
        iShowBMP2(SCREEN_WIDTH - 315, SCREEN_HEIGHT - 320, "project images\\cavee.png",0x000000);
    }
}

void level_3_popup()
{
    iShowBMP(950,10,"project images\\levels\\level 3.png");
    if(quick_text2)
    {
        if (start_time == 0)
        {
            start_time = clock(); // Set the start time when quick_text is first true
        }
        iText(425,390,"LEVEL 3\n\n",GLUT_BITMAP_TIMES_ROMAN_24);
        if ((clock() - start_time) / CLOCKS_PER_SEC >= 1)
        {
            quick_text2 = false;
            start_time = 0;
        }
        iShowBMP2(SCREEN_WIDTH - 320, 80, "project images\\cavee.png",0x000000);
    }
}

void highscores()
{
    iSetColor(255,255,255);
    iText(455, 290, "TOP SCORES :", GLUT_BITMAP_HELVETICA_12);
    int y = 275;
    for (int i = 0; i < len; i++)
    {
        char scoreText[100];
        sprintf(scoreText, "%d. %s: %d", i + 1, high_score[i].name, high_score[i].score);
        
        iSetColor(255,0,0);
        iText(455, y, scoreText, GLUT_BITMAP_HELVETICA_12);
        y -= 15;
    }
}

void draw_food()
{
    iSetColor(0, 0, 0); // Set color to black
    iFilledCircle(foodX, foodY, food_radius);
    if(food_type == 1)
    {
        iShowBMP2(foodX-17,foodY-17,food_img1[eaten],0x000000);
    }
    if(food_type == 2)
    {
        iShowBMP2(foodX-28,foodY-28,food_img2[eaten],0x000000);
    }
}

void show_score()
{
    iSetColor(255, 255, 255);
    iText(15, 665, "SCORE :", GLUT_BITMAP_TIMES_ROMAN_24);
    sprintf(Score, "%d", score);
    iText(120, 665, Score, GLUT_BITMAP_TIMES_ROMAN_24);
}

int counter = 1;
int sound_counter = 1;
void gameover()
{
    if(gameover_sound)
    {
        if(sound_counter == 1)
        {
            // PLAY SOUND ACTIVATE
            PlaySoundA("sound effects\\game_obher.wav", NULL, SND_ASYNC);
            printf("play sound.\n");
            gameover_sound = false;
            sound_counter++;
        }
    }
    move = !move;
    iShowBMP(0,0,"project images\\gameover_screen.png");
    iSetColor(255, 255, 255); 
    sprintf(Score, "%d", score);
    iText(490, 340, Score, GLUT_BITMAP_TIMES_ROMAN_24);
    if(counter == 1)
    {
        inputMode = true;
        printf("ball %d\t",counter);
    }
    if(name_entered)
    {
        if (start_time == 0)
        {
            start_time = clock(); // Set the start time when quick_text is first true
        }
        iText(660, 430, "Name Entered", GLUT_BITMAP_TIMES_ROMAN_10);

        if ((clock() - start_time) / CLOCKS_PER_SEC >= 1)
        {
            name_entered = false;
            start_time = 0;
        }
    }
    counter++;

    char displayText[100];
    iSetColor(255,255,255);
    sprintf(displayText, "Your name:   %s", userInput);
    iText(275,425, displayText, GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255,0,0);
    iRectangle(397,415,250,40);
}   

void level_load()
{
    move = true;
    inputMode = false;
    counter = 1;
    music_on = false;
    level_up_sound = 1;
    level_up_sound2 = 1;
}

void menu()
{
    if(homepage_play){
        iShowBMP(0,0,"project images\\homepage\\homepage_play.png");                      
    }
    if(homepage_settings){
        iShowBMP(0,0,"project images\\homepage\\homepage_settings.png");                      
    }
    if(homepage_exit){
        iShowBMP(0,0,"project images\\homepage\\homepage_exit.png");                      
    }
    if(homepage_instructions){
        iShowBMP(0,0,"project images\\homepage\\homepage_instructions.png");                      
    }
    if(homepage_score){
        iShowBMP(0,0,"project images\\homepage\\homepage_score.png");                      
    }
    if(pause_button2){
        iShowBMP(SCREEN_WIDTH - 70,SCREEN_HEIGHT - 75,"project images\\pause button glow.png");                      
    }
    if(back_button){
        iShowBMP(0,0,"project images\\instructions_page_back_button_glow.png");                      
    }
    if(glowing_ball){
        iShowBMP(0,0,"project images\\homepage\\homepage_redball.png");
    }
}

void show_high_score()
{
    // Displaying the high scores
    int y = 425;
    for (int i = 0; i < len; i++) {
        char scoreText[100];
        iSetColor(0,255,0);
        sprintf(scoreText, "(%d) %s ", i + 1, high_score[i].name);
        iText(370, y, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
        sprintf(scoreText, "%d",high_score[i].score);
        iText(670, y, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
        y -= 30;
    }
}

void music_off()
{
    PlaySoundA("sound effects\\blank.wav", NULL, SND_ASYNC);
}

void music()
{
    PlaySoundA("sound effects\\homepage sound.wav", NULL, SND_ASYNC);
    printf("m_music ");
}
int wow = 1;
void iDraw()
{
    iClear();
    iSetColor(255,255,255);
    iRectangle(50,50,SCREEN_WIDTH-100, SCREEN_HEIGHT-100);
    iRectangle(52,52,SCREEN_WIDTH-104, SCREEN_HEIGHT-104);
    
    if(game_state == 0) {
        iShowBMP(0,0,homepage);
    }
    if(game_state == 1) iShowBMP(0,0,instructions);
    if(game_state == 2) iShowBMP(0,0,pause_menu);
    if(game_state == 3)
    {
        // printf("wow\n");
        iShowBMP(0,0,"project images\\leaderboard.png");
        read_high_scores();
        show_high_score();
        iSetColor(255,255,255);
        iText(365, 235, "press ESC to return to menu", GLUT_BITMAP_TIMES_ROMAN_24);
    }

    if(game_state == 4)
    {
        if(music_tick)
        {
            iShowBMP(0,0,"project images\\settings\\settings page music.png");
            // if(music_on)

            if(wow == 1)
            {
                printf("\nmusic on 2\n");
                music_on = true;
                music_on_count = 1;
                wow++;
            }
        }
        
        else
        {
            iShowBMP(0,0,"project images\\settings\\settings page music off.png");
            music_on = false;
            if(music_off_count == 1)
            {
                if(!music_on)
                {
                    music_off();
                    music_off_count++;
                    printf("\nmusic off\n");
                    wow = 1;
                }
            }
            // music_off();
        }

        iSetColor(255,255,255);
        iText(365, 235, "press ESC to return to menu", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    
    if(game_state == 11 || game_state == 12 || game_state == 13)
    {

        iShowBMP(875,20,"project images\\levels\\level text.png");
        
        if(game_state == 11)
        {
            level_1();
            iShowBMP(950,10,"project images\\levels\\level 1.png");
        }

        if(game_state == 12)
        {
            level_2();
            level_2_popup();
            draw_rec();
        }

        if(game_state == 13)
        {
            level_3_popup();
            level_3();
            draw_rec();
            draw_obstacle();
        }

        draw_snake();
        draw_food();
        show_score();   // top left

        // pause button
        iShowBMP(SCREEN_WIDTH - 70,SCREEN_HEIGHT - 75,"project images\\pause button.png");
    }

    if (game_over)
    {
        gameover();
        // PlaySoundA("game_obher.wav", NULL, SND_LOOP | SND_ASYNC);
    }
    if(show_hsc)
    {
        highscores();
    }
    menu();

    if(music_on_count == 1)
    {
        if(music_on)
        {
            music();
            printf("+ music on 1\n");
            music_on_count++;
            music_off_count = 1;
        } 
    }

    if(music_off_count == 1)
    {
        if(!music_on)
        {
            music_off();
            music_off_count++;
            printf("m_menu - play - music off\n");
        }
    }

    if(game_state == 11 || game_state == 12 || game_state == 13)
        iText(460,20,"'space_bar' -> pause",GLUT_BITMAP_HELVETICA_10);
    
}

void iMouseMove(int mx, int my) {}
 
void iPassiveMouseMove(int mx, int my)
{
    if(game_state == 0  &&                              // play button glow
        mx >= 400 && mx <= 590 && my >= 475 && my <= 545) 
        {
            homepage_play = true;
        }
        else homepage_play = false;

    if(game_state == 0 &&                               // settings button glow
        mx >= 400 && mx <= 595 && 
        my >= 385 && my <= 445) 
        {
            homepage_settings = true;
        }
        else homepage_settings = false;

    if(game_state == 0 &&                               //  exit button glow
        mx >= 400 && mx <= 595 && my >= 290 && my <= 355)
        {
            homepage_exit = true;
        }
        else homepage_exit = false;

    if(game_state == 0 &&                               //  instructions button glow
        mx >= 720 && mx <= 930 && my >= 455 && my <= 490)
        {
            homepage_instructions = true;
        }
        else homepage_instructions = false;
    
    if(game_state == 0 &&                               //  score button glow
        mx >= 780 && mx <= 865 && my >= 390 && my <= 420)
        {
            homepage_score = true;
        }
        else homepage_score = false;
    
    if(game_state == 1 && mx >= 30 && mx <= 160 && my >= 610 && my <= 665)  // instruction page, back button glow
    {
        back_button = true;
    } else back_button = false;

    if((game_state == 11 || game_state == 12 || game_state == 13) &&        // pause button glow
        mx >= 930 && mx <= 960 && 
        my >= 630 && my <= 660) 
    {
            pause_button2 = true;                            
    } else pause_button2 = false;

    if(game_state == 0 && mx >= 650 && mx <= 690 && my >= 15 && my <= 55)
    {
        glowing_ball = true;
    } else glowing_ball = false;

    
}

void reset()
{
    printf("called\n");
    music_on = true;
    music_off_count = 1;
    level_up_sound = 1;
    level_up_sound2 = 1;
    music_on_count = 1;
    quick_text = true;
    quick_text2 = true;
    snake_size = 5;
    score = 0;
    small_food_eaten = 0;
    game_over = false;
    sound_counter = 1;
    positioning = 1;
    positioning2 = 1;
    direction = 4;
    // game_state = 0;     
}


void iMouse(int button, int state, int mx, int my)
{
    // mouse click co-ordinates
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // to find mouse click position
        int x = mx;
        int y = my;
        printf("(x,y) = %d, %d\n", mx, my);
    }
        

 /*
 ##################################################
    HOME PAGE FUNCTIONING WITH MOUSE
 ##################################################
 */

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(game_state == 0  && 
        mx >= 400 && mx <= 590 && 
        my >= 475 && my <= 545) 
        {
            level_load();
            game_state = 11;                                // homepage to level 1
        }

        if(game_state == 0 &&                               // homepage to settings
        mx >= 400 && mx <= 595 && 
        my >= 385 && my <= 445) 
        {
            game_state = 4;
            // music_tick = true;
        }

        if(game_state == 0 &&                        // homepage to instructions
        mx >= 720 && mx <= 930 && my >= 455 && my <= 490) game_state = 1;     
        
        if(game_state == 0 &&                        // homepage to leaderboard
        mx >= 780 && mx <= 865 && my >= 390 && my <= 420)
        {
            game_state = 3;     
            // printf("wow\n");
        } 
        
        if(game_state == 0 &&                       // homepage to exit
        mx >= 400 && mx <= 595 && my >= 290 && my <= 355) 
        exit(0);

        if(game_state == 1 && 
        mx >= 30 && mx <= 160 && 
        my >= 610 && my <= 665) {
            game_state = 0;     // instructions to homepage
            music_on = true;
            music_off_count = 1;
            music_on_count = 1;
            quick_text = true;
            level_up_sound = 1;
            level_up_sound2 = 1;
        }
        
        if((game_state == 11 || game_state == 12 || game_state == 13) && 
        mx >= 930 && mx <= 960 && 
        my >= 630 && my <= 660) 
        {
            move = false;
            game_state = 2;                         // level 1 or level 2 to pause menu      
        }
        

        if(game_over &&                              // gameover to exit
        mx >= 605 && mx <= 805 && my >= 200 && my <= 255) 
        {
            exit(0);
            inputMode = false;

        }

        if(game_over &&                             // gameover to level 1
        mx >= 200 && mx <= 395 && my >= 200 && my <= 255) 
        {
            inputMode = false;
            name_entered = false;
            show_hsc = false;
            counter = 1;

            move = true;

            reset();
            game_state = 11;
        }
        
        if(game_over &&                             // gameover to homepage
        mx >= 420 && mx <= 595 && my >= 45 && my <= 105) 
        {
            inputMode = false;
            name_entered = false;
            show_hsc = false;
            counter = 1;
            reset();
            game_state = 0;
            level_up_sound = 1;
            level_up_sound2 = 1;
        }

        if(game_state == 2 &&                       // pause menu to level 1 or level 2
        mx >= 430 && mx <= 570 && my >= 365 && my <= 415)
        {
            move = true;
            if(score <= level_2_score)      game_state = 11;
            else if(score <= level_3_score) game_state = 12;
            else                            game_state = 13;
        }

        if(game_state == 2 &&                             // pause menu to homepage
        mx >= 430 && mx <= 570 && my >= 295 && my <= 340) 
        {
            reset();
            game_state = 0;
            level_up_sound = 1;
            level_up_sound2 = 1;
        }

        if(game_state == 2 &&                             // gameover to exit
        mx >= 430 && mx <= 570 && my >= 230 && my <= 280) 
        exit(0);

        if(game_state == 4 && mx >= 265 && mx <= 290 && my >= 330 && my <= 355)
        {
            music_tick = true;      // settings music on
        }

        if(game_state == 4 && mx >= 265 && mx <= 290 && my >= 270 && my <= 300)
        {
            music_tick = false;     // settings music off
        }

        if(game_state == 4 && mx >= 270 && mx <= 485 && my >= 30 && my <= 145)
        {
            printf("lvl 1\n");      // settings to level 1
            
            level_load();
            game_state = 11;
        }

        if(game_state == 4 && mx >= 510 && mx <= 735 && my >= 30 && my <= 145)
        {
            printf("lvl 2\n");      // settings to level 2
            level_load();
            game_state = 12;
        }
        if(game_state == 0 && mx >= 650 && mx <= 690 && my >= 15 && my <= 55)
        {
            printf("lvl 3\n");      // settings to level 2
            level_load();
            game_state = 13;        // homepage to level 3
        }
    }
}

/*
#################################################
    SNAKE MOVEMENT WITH KEYBOARD (W-A-S-D)
#################################################
*/

void iKeyboard(unsigned char key)
{
    if ((key == 'w' || key == 'W') && direction != 2)
    {
        direction = 8;      // Move up
    }   
        
    if ((key == 'a' || key == 'A') && direction != 6)
    {
        direction = 4;      // Move left
        left = true;
        right = false;
    }    
    
    if ((key == 's' || key == 'S') && direction != 8)     
    {
        direction = 2;      // Move down
    }

    if ((key == 'd' || key == 'D') && direction != 4)
    {
        direction = 6;      // Move right
        right = true;
        left = false;
    }    

    if(key == ' ') 
    {
        move = !move;
        // printf("space clicked %d\n",move);
    }
    
    if(game_state == 3 && key == 27)
    {
        game_state = 0;
    }
    if(game_state == 4 && key == 27)
    {
        game_state = 0;
    }

    if (inputMode)
    {
        if (key == '\r')
        { // Enter key to finalize input
            printf("pressed enter");
            inputMode = false;
            name_entered = true;
            show_hsc = true;
            read_high_scores();
            add_new_score();
            write_high_scores();
            highscores();
            
        }
        else if (key == '\b')
        { // Backspace key to delete character
            if (inputIndex > 0)
            {
                inputIndex--;
                userInput[inputIndex] = '\0';
            }
        }
        else if (inputIndex < MAX_NAME_LENGTH - 1)
        {
            userInput[inputIndex] = key;
            inputIndex++;
            userInput[inputIndex] = '\0';
        }
    }
}

/*
#################################################
    SNAKE MOVEMENT WITH KEYBOARD (arrow keys)
#################################################
*/

void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_UP && direction != 2)       direction = 8;      // Move up
    if (key == GLUT_KEY_LEFT && direction != 6)
    {
        direction = 4;      // Move left
        left = true;
        right = false;
    }    
        
    if (key == GLUT_KEY_DOWN && direction != 8)     direction = 2;      // Move down
    if (key == GLUT_KEY_RIGHT && direction != 4)
    {
        direction = 6;      // Move right
        right = true;
        left = false;
    }    
}

int main()
{
    initialize_snake();

    iSetTimer(75, move_snake); // Move the snake every 100ms
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    return 0;
}