 /* Author: Hulebrt Zeng
 * Partner(s) Name (if applicable):  
 * Lab Section: 021
 * Assignment: Lab #9  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://youtu.be/F3NdvTy8Wds
 */ 
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

enum TL_States { TL_SMStart, TL_State1, TL_State2, TL_State3 } TL_State;
enum BL_States { BL_SMStart, BL_State1, BL_State2 } BL_State;
enum CL_States { CL_SMStart, CL_Init } CL_State;

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char i = 0;
unsigned char j = 0;

void ThreeLED() {
    switch(TL_State) {
        case TL_SMStart:
            TL_State = TL_State1;
            break;
        case TL_State1:
            if(i < 3) {
                threeLEDs = 0x01;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State2;
            }
            break;
        case TL_State2:
            if(i < 3) {
                threeLEDs = 0x02;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State3;
            }
            break;
        case TL_State3:
            if(i < 3) {
                threeLEDs = 0x04;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State1;
            }
            break;
        default:
            TL_State = TL_SMStart;
            break;
    }
}

void BlinkLED() {
    switch(BL_State) {
        case BL_SMStart:
            BL_State = BL_State1;
            break;
        case BL_State1:
            if(j < 10) {
                blinkingLED = 0x08;
                ++j;
            } else {
                j = 0;
                BL_State = BL_State2;
            }

            break;
        case BL_State2:
            if(j < 10) {
                blinkingLED = 0x00;
                ++j;
            } else {
                j = 0;
                BL_State = BL_State1;
            }
            break;
        default:
            BL_State = BL_SMStart;
            break;
    }
}

void CombineLED() {
    switch(CL_State) {
        case CL_SMStart:
            CL_State = CL_Init;
            break;
        case CL_Init:
            PORTB = threeLEDs | blinkingLED;
            break;
        default:
            CL_State = CL_SMStart;
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    TimerSet(100);
    TimerOn();
    /* Insert your solution below */
    while (1) {
        while(!TimerFlag);
        ThreeLED();
        BlinkLED();
        CombineLED();
        TimerFlag = 0;
    }
    return 1;
}
