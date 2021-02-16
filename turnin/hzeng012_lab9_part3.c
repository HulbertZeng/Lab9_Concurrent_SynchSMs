 /* Author: Hulbert Zeng
 * Partner(s) Name (if applicable):  
 * Lab Section: 021
 * Assignment: Lab #9  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://youtu.be/Srp83QBTJ9k
 */ 
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

/* 
   Note fo TA: There was an LED error in the demo video and so I tried fixing it.
   When I did, the button and speaker stopped working properly despite me not changing 
   any of the code for the button or speaker logic. When I tried reverting the code 
   back to normal, the button pressing still was not working correctly but the LED error 
   did return, so I suspect my speaker might have become faulty shortly after I made the 
   demo video. Thus, I was unable to make a new demo video, reflecting the fix I tried 
   applying to my code.
*/

enum TL_States { TL_SMStart, TL_State1, TL_State2, TL_State3 } TL_State;
enum BL_States { BL_SMStart, BL_State1, BL_State2 } BL_State;
enum CL_States { CL_SMStart, CL_Init } CL_State;

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned int i = 0;
unsigned int j = 0;

void ThreeLED() {
    switch(TL_State) {
        case TL_SMStart:
            TL_State = TL_State1;
            break;
        case TL_State1:
            if(i < 150) {
                threeLEDs = 0x01;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State2;
            }
            break;
        case TL_State2:
            if(i < 150) {
                threeLEDs = 0x02;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State3;
            }
            break;
        case TL_State3:
            if(i < 150) {
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
            if(j < 500) {
                blinkingLED = 0x08;
                ++j;
            } else {
                j = 0;
                BL_State = BL_State2;
            }

            break;
        case BL_State2:
            if(j < 500) {
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
            PORTB += threeLEDs | blinkingLED;
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
    TimerSet(2);
    TimerOn();
    TL_State = TL_SMStart;
    BL_State = BL_SMStart;
    CL_State = CL_SMStart;
    /* Insert your solution below */
    while (1) {
        while(!TimerFlag);
        ThreeLED();
        BlinkLED();
        CombineLED();
        if((~PINA) & 0x04) {
            PORTB = PORTB | 0x10;
        }
        TimerFlag = 0;
    }
    return 1;
}
