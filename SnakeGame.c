#include <stdio.h>
#include <conio.h>
#include <pthread.h>
#include "draw.h"
#include "console.h"
#include "random.h"

typedef struct
{
    int x, y;
} position;

void *myThread(void *vargp);
void GetController(void);
void SetFood(void);
int Collision(position, position);

int updown, leftright = 1;
position SnakeHeadPos, FoodPos;
position SnakeBodyPos[1525]; int SnakeBodyIndex;
int Score;
int GAMEENDED;

int main()
{
    pthread_t thread_id;
    int i, result, snakeLen = 0;

    Play:

    Hidecursor();
    srand(time(0));

    gotoxy(wherey(), 2);
    printf("Snake Game");
    gotoxy(wherey(), 54);
    printf("Score : 0\n");
    Rect(119,28);

    //Create thread
    pthread_create(&thread_id, NULL, myThread, NULL);
    //First time food
    SetFood();

    //Snake Position
    SnakeHeadPos.x =  Random(3,115);
    SnakeHeadPos.y =  Random(3,25);
    gotoxy(SnakeHeadPos.y, SnakeHeadPos.x);

    do
    {   
        if(SnakeBodyIndex > 1000)
        {
            gotoxy(0, 110);
            textcolor(LIGHTRED);
            printf("YOU LOST");
            textcolor(WHITE);
            goto GAME_END;
        }

        if((wherex()-1 == FoodPos.x || wherex()-2 == FoodPos.x) && wherey() == FoodPos.y)
        {
            SnakeHeadPos.x = wherex();
            SnakeHeadPos.y = wherey();

            SetFood();
            
            Score++;
            gotoxy(0, 62);
            printf("%d",Score);

            if(Score == 10)
            {
                gotoxy(0, 110);
                textcolor(LIGHTGREEN);
                printf("YOU WON");
                textcolor(WHITE);
                goto GAME_END;
            }

            gotoxy(SnakeHeadPos.y, SnakeHeadPos.x);
        }

        printf("\x08\x20\x08\x08\x20\x08");

        //Add new snake body
        //Snake Body position
        SnakeBodyPos[SnakeBodyIndex].x = wherex();
        SnakeBodyPos[SnakeBodyIndex].y = wherey();
        SnakeBodyIndex++;

        //Snake Body Graphic
        printf("\xDB\xDB");
        printf("\x08");
        if(updown) printf("\x08");
        else if(leftright == -1) printf("\x08\x08");

        //Borders teleportation and snake controller
        if(updown == -1 && wherey() <= 2)
            gotoxy(28 + updown, wherex() + leftright);
        else if(updown == 1 && wherey() >= 27)
            gotoxy(1 + updown, wherex() + leftright);
        else if(leftright == -1 && wherex() <= 1)
            gotoxy(wherey() + updown, 116 + leftright);
        else if(leftright == 1 && wherex() > 115)
            gotoxy(wherey() + updown, 0 + leftright);
        else
            gotoxy(wherey() + updown, wherex() + leftright);

        //Snake Head
        SnakeHeadPos.x = wherex();
        SnakeHeadPos.y = wherey();

        textcolor(LIGHTGREEN);
        printf("\xDB\xDB");
        textcolor(WHITE);

        for(i = 0; i < SnakeBodyIndex; i++)
        {
            if(Collision(SnakeHeadPos, SnakeBodyPos[i]))
            {
                gotoxy(0, 110);
                textcolor(LIGHTRED);
                printf("YOU LOST");
                textcolor(WHITE);
                goto GAME_END;
            }
        }

        delay(100);

    }while(1);

    GAME_END:
    GAMEENDED = 1;

    gotoxy(13,52);
    textbackground(LIGHTRED);
    printf("Press Any Key");
    textbackground(BLACK);

    pthread_join(thread_id, NULL);
    //pthread_cancel(thread_id);

    delay(100);

    clrscr();

    Showcursor();

    gotoxy(10,55);
    printf("1.Play Again\n");
    gotoxy(wherey(),55);
    printf("0.Exit\n");
    gotoxy(wherey(),55);
    printf("Choice : ");
    //scanf("%d",&result);
    result = getch();
    //0 key equal to 48 in ASCII CODE
    result = result == 48 ? 0 : 1;

    if(result)
    {
        clrscr();
        SnakeBodyIndex = Score = GAMEENDED = 0;
        goto Play;
    }
    return 0;
}

//User Controller
void GetController()
{
    //UP   : 72 
    //Down : 80
    //Left : 75
    //Right: 77
    int key;

    key = getch();

    //UP key || W key || w key
    if(key == 72 || key == 87 || key == 119)
    {
        if(updown == 1) return; //you can't go the opposite way
        updown = -1;
        leftright = 0;
    }
    //DOWN key || S key || s key
    else if(key == 80 || key == 83 || key == 115)
    {
        if(updown == -1) return; //you can't go the opposite way
        updown = 1;
        leftright = 0;
    }
    //LEFT key || A key || a key
    else if(key == 75 || key == 65 || key == 97)
    {
        if(leftright == 1) return; //you can't go the opposite way
        updown = 0;
        leftright = -1;
    }
    //RIGHT key || D key || d key
    else if(key == 77 || key == 68 || key == 100)
    {
        if(leftright == -1) return; //you can't go the opposite way
        updown = 0;
        leftright = 1;
    }
}

void *myThread(void *vargp)
{
    while(!GAMEENDED)
    {
        GetController();
    }
    return NULL;
}

void SetFood()
{
    int i;

    do
    {
        FoodPos.x = Random(3,116);
        FoodPos.y = Random(3,26);

        for(i=0; i<SnakeBodyIndex; i++)
        {
            if((FoodPos.x == SnakeBodyPos[i].x || FoodPos.x == SnakeBodyPos[i].x+1) && FoodPos.y == SnakeBodyPos[i].y)
                break;
        }

    }while(((SnakeHeadPos.x == FoodPos.x || SnakeHeadPos.x+1 == FoodPos.x) && SnakeHeadPos.y == FoodPos.y) || i != SnakeBodyIndex);

    gotoxy(FoodPos.y, FoodPos.x);
    textcolor(LIGHTRED);
    printf("\xFE");
    textcolor(WHITE);
}

int Collision(position pos1, position pos2)
{
    if((pos1.x == pos2.x || pos1.x == pos2.x+1 || pos1.x+1 == pos2.x || pos1.x+1 == pos2.x+1)
    && pos1.y == pos2.y)
        return 1;
    return 0;
}