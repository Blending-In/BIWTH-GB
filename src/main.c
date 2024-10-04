#include <stdio.h>
#if GB
#include <gb/gb.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <rand.h>
#else
#include <stdlib.h>
#include <time.h>
#endif

// Sprites
#if GB
#include "sprites-gb/homie.h"
#include "sprites-gb/homer.h"
#include "sprites-gb/left_arrow.h"
#include "sprites-gb/right_arrow.h"
#include "sprites-gb/up_arrow.h"
#include "sprites-gb/down_arrow.h"
#endif

void drawHomie(int x, int y, int curHomie) { 
    #if GB
    set_sprite_tile(curHomie, 0);
    move_sprite(curHomie, x, y);
    #endif
}

void drawHomer(int x, int y, int curHomer) {
    #if GB
    set_sprite_tile(curHomer, 4);
    move_sprite(curHomer, x, y);
    #endif
}

void drawArrows(int hide) {
    // left, right, down, up in order
    int arrowX = 56;
    int arrowI = 0;
    if (hide == 1) {
        for (arrowI = 0; arrowI < 4; arrowI++) {
            #if GB
            move_sprite(arrowI + 7, 250, 100);
            #endif
        }
        return;
    }
    
    for (arrowI = 0; arrowI < 4; arrowI++) {
        #if GB
        if (arrowI == 0) {
            set_sprite_tile(arrowI + 7, 6);
        } else if (arrowI == 1) {
            set_sprite_tile(arrowI + 7, 8);
        } else if (arrowI == 2) {
            set_sprite_tile(arrowI + 7, 10);
        } else if (arrowI == 3) {
            set_sprite_tile(arrowI + 7, 12);
        }
        move_sprite(arrowI + 7, arrowX, 96);
        #endif
        arrowX += 16;
    } 
}

int randIndex(){ // 0-3
    int randNum = rand() % 4;
    while (randNum < 0 || randNum > 4) {
        randNum = rand() % 4;
    }

    return randNum;
}

int checkForHomie(int pos, int homieArray[4]) {
    if (homieArray[pos] == 0) {
        return 1;
    } else {
        return 0;
    }
}

void enterGameOver(
    #if GB
    font_t ibm_font
    #endif
) {
    int homieI, scoreDigits;
    // clear screen
    // clear homies
    for (homieI = 0; homieI < 4; homieI++) {
        drawHomie(200, 100, homieI);
    }
    drawHomer(200, 100, 3); // clear homer
    drawArrows(1);
    // clear arrows

    #if GB
    font_set(ibm_font);
    gotoxy(0, 0);
    // fill whole screen with spaces (including height)
    for (homieI = 0; homieI < 18; homieI++) {
        for (scoreDigits = 0; scoreDigits < 20; scoreDigits++) {
            printf(" ");
        }
    }
    #endif
}

void main(void)
{
    #if GB
    UWORD seed;
    #else
    srand(time(NULL));
    #endif
    int score = 0;
    int lastScore = 0; // used to know which characters to clean up if it drops below a certain digit again (3 -> 2. clear 3)
    int input, inputI, homieI;
    int inputArray[7], inputs[7];
    int homieArray[4] = {0, 0, 0, 0}; // 0 = homie, 1 = homer, 3 = peter, 4 = that one guy i forgot his name
    int scoreDigits = 0;
    int lastScoreDigits = 0;
    int scoreTemp = score;
    int lastScoreTemp = lastScore;
    int currentState = 0;
    int curHomer = 3;
    int lastPos = 3;
    /*
        * 0 = title screen
        * 1 = game
        * 2 = game over
    */
    #if GB
    font_t ibm_font; // TODO: Remove fonts that aren't used to save space

    SPRITES_8x16;
    // Homies
    set_sprite_data(0, 0, homie);
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 0);
    set_sprite_tile(2, 0);
    set_sprite_tile(3, 0);

    // Homer
    set_sprite_data(4, 4, homer);
    // copies until we add peter and larry
    set_sprite_tile(4, 4);
    set_sprite_tile(5, 4);

    // Arrows
    set_sprite_data(6, 6, left_arrow);
    set_sprite_data(8, 8, right_arrow);
    set_sprite_data(10, 10, down_arrow);
    set_sprite_data(12, 12, up_arrow);

    seed = DIV_REG;
    seed |= (UWORD)DIV_REG << 8;
    initarand(seed);

    homieArray[3] = 1;

    font_init();

    ibm_font = font_load(font_ibm);  /* 96 tiles */
    
    color(WHITE, DKGREY, SOLID);
    
    mode(get_mode() | M_NO_SCROLL);

    font_set(ibm_font);
    printf("Score: %d\n", score);

    input = joypad();
    #endif

    while(1) {
        #if GB
        wait_vbl_done();
        input = joypad();

        inputArray[0] = J_LEFT;
        inputArray[1] = J_RIGHT;
        inputArray[2] = J_DOWN;
        inputArray[3] = J_UP;
        inputArray[4] = J_A;
        inputArray[5] = J_B;
        inputArray[6] = J_START;
        #endif

        for (inputI = 0; inputI < 7; inputI++) {
            // 0 = not down, 1 = (just)pressed, 2 = down, 3 = (just)released
            if (input & inputArray[inputI]) {
                if ((inputs[inputI] == 0 || inputs[inputI] == 3) && inputs[inputI] != 2) {
                    inputs[inputI] = 1;
                } else if (inputs[inputI] == 1) {
                    inputs[inputI] = 2;
                }
            } else {
                if (inputs[inputI] == 2 || inputs[inputI] == 1) {
                    inputs[inputI] = 3;
                } else {
                    inputs[inputI] = 0;
                }
            }
        }

        switch (currentState)
        {
            case 0:
                // title screen
                #if GB
                font_set(ibm_font);
                gotoxy(0, 0);
                printf("Blending In\nWith The\nHomies\n\nPress Start to play");
                #endif
                score = 0;

                if (inputs[6] == 1) {
                    currentState = 1;
                    enterGameOver(
                        #if GB 
                        ibm_font 
                        #endif
                    );
                }
                
                break;
            case 1:
                for (homieI = 0; homieI < 4; homieI++) {
                    if (inputs[homieI] == 1) {
                        if (checkForHomie(homieI, homieArray) == 1) {
                            currentState = 2;
                            enterGameOver(
                                #if GB 
                                ibm_font 
                                #endif
                            );

                            break;
                        } else {
                            score++;
                            homieArray[homieI] = 0;
                            curHomer = randIndex();
                            while (curHomer == lastPos) {
                                curHomer = randIndex();
                            }
                            homieArray[curHomer] = 1;
                            lastPos = curHomer;
                        }
                    }
                }

                #if GB
                font_set(ibm_font);
                gotoxy(0, 0);

                printf("Score: ");

                for (scoreDigits = 0; scoreDigits < 4; scoreDigits++) {
                    printf(" ");
                }

                gotoxy(7, 0);
                printf("%d", score);
                #endif
                
                for (homieI = 0; homieI < 4; homieI++) {
                    if (homieArray[homieI] == 0) {
                        drawHomie(56 + homieI * 16, 80, homieI);
                    } else if (homieArray[homieI] == 1) {
                        drawHomer(56 + homieI * 16, 80, homieI);
                    }
                }

                drawArrows(0);

                lastScore = score;

                break;
            case 2:
                #if GB
                font_set(ibm_font);
                gotoxy(0, 0);

                printf("Game Over\nWomp Womp...\n");

                printf("Score: %d", score);

                printf("\n\nPress Start to go to\nthe title screen");
                #endif

                if (inputs[6] == 1) {
                    enterGameOver(
                        #if GB 
                        ibm_font 
                        #endif
                    );
                    currentState = 0;
                }

                break;
        }

        #if GB
        SHOW_SPRITES;
        #endif
    }
}
