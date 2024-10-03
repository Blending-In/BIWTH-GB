#include <stdio.h>
#include <gb/gb.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>

// Sprites
#include "sprites/homie.h"

void drawHomie(int x, int y, int curHomie) { 
    set_sprite_tile(curHomie, 0);
    move_sprite(curHomie, x, y);
}

void main(void)
{
    int score = 0;
    int input, inputI, homieI;
    int inputArray[7], inputs[7];
    int homieArray[4] = {0, 0, 0, 0}; // 0 = homie, 1 = homer, 3 = peter, 4 = that one guy i forgot his name
    int currentState = 1;
    /*
        * 0 = title screen
        * 1 = game
        * 2 = game over
    */
    font_t ibm_font, italic_font, min_font; // TODO: Remove fonts that aren't used to save space

    SPRITES_8x16;
    // setup 4 clones of the homie sprite
    set_sprite_data(0, 0, homie);
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 0);
    set_sprite_tile(2, 0);
    set_sprite_tile(3, 0);

    font_init();

    ibm_font = font_load(font_ibm);  /* 96 tiles */
    italic_font = font_load(font_italic);   /* 93 tiles */
    
    color(WHITE, DKGREY, SOLID);
    
    min_font = font_load(font_min);

    mode(get_mode() | M_NO_SCROLL);

    font_set(ibm_font);
    printf("Score: %d\n", score);

    input = joypad();

    while(1) {
        wait_vbl_done();
        input = joypad();

        inputArray[0] = J_LEFT;
        inputArray[1] = J_RIGHT;
        inputArray[2] = J_UP;
        inputArray[3] = J_DOWN;
        inputArray[4] = J_A;
        inputArray[5] = J_B;
        inputArray[6] = J_START;

        for (inputI = 0; inputI < 7; inputI++) {
            if (input & inputArray[inputI]) {
                if (inputs[inputI] == 0) {
                    inputs[inputI] = 1;
                } else {
                    inputs[inputI] = 2;
                }
            } else if (input & inputArray[inputI]) {
                if (inputs[inputI] == 2) {
                    inputs[inputI] = 3;
                } else {
                    inputs[inputI] = 0;
                }
            } else if (input & inputArray[inputI]) {
                inputs[inputI] = 0;
            } else {
                inputs[inputI] = 0;
            }
        }

        if (inputs[0] == 1) {
            score++;
        } else if (inputs[1] == 1) {
            score--;
        }

        font_set(ibm_font);
        gotoxy(0, 0);
        printf("Score: %d\n", score);

        for (homieI = 0; homieI < 4; homieI++) {
            if (homieArray[homieI] == 0) {
                drawHomie(56 + homieI * 16, 80, homieI);
            }
        }
        
        SHOW_SPRITES;
    }
}
