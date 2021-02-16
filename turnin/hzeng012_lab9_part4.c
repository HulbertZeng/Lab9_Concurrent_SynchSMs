 /* Author: Hulbert Zeng
 * Partner(s) Name (if applicable):  
 * Lab Section: 021
 * Assignment: Lab #9  Exercise #4
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: 
 */ 
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

enum TL_States { TL_SMStart, TL_State1, TL_State2, TL_State3 } TL_State;
enum BL_States { BL_SMStart, BL_State1, BL_State2 } BL_State;
enum CL_States { CL_SMStart, CL_Init } CL_State;
enum FC_States { FC_SMStart, FC_Init, FC_Inc, FC_Dec } FC_State;

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned int i = 0;
unsigned int j = 0;
unsigned int k = 0;
unsigned int freq = 2;

void ThreeLED() {
    switch(TL_State) {
        case TL_SMStart:
            TL_State = TL_State1;
            break;
        case TL_State1:
            if(i < 300) {
                threeLEDs = 0x01;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State2;
            }
            break;
        case TL_State2:
            if(i < 300) {
                threeLEDs = 0x02;
                ++i;
            } else {
                i = 0;
                TL_State = TL_State3;
            }
            break;
        case TL_State3:
            if(i < 300) {
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
            if(j < 1000) {
                blinkingLED = 0x08;
                ++j;
            } else {
                j = 0;
                BL_State = BL_State2;
            }

            break;
        case BL_State2:
            if(j < 1000) {
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

void FrequencyChanger() {
    unsigned char inc = (~PINA) & 0x01;
    unsigned char dec = (~PINA) & 0x02;
    switch(FC_State) {
        case FC_SMStart:
            FC_State = FC_Init;
            break;
        case FC_Init:
            if(inc) {
                ++freq;
                FC_State = FC_Inc;
            } else if(dec && freq > 1) {
                --freq;
                FC_State = FC_Dec;
            }
            break;
        case FC_Inc:
            if(!inc) { FC_State = FC_Init; }
            break;
        case FC_Dec:
            if(!dec) { FC_State = FC_Init; }
            break;
        default:
            FC_State = FC_SMStart;
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    TimerSet(1);
    TimerOn();
    TL_State = TL_SMStart;
    BL_State = BL_SMStart;
    CL_State = CL_SMStart;
    FC_State = FC_SMStart;
    /* Insert your solution below */
    while (1) {
        while(!TimerFlag);
        ThreeLED();
        BlinkLED();
        CombineLED();
        FrequencyChanger();
        if(((~PINA) & 0x04) && (k == freq)) {
            PORTB = PORTB | 0x10;
        }
        ++k;
        if(k > freq) {
            k = 0;
        }
        TimerFlag = 0;
    }
    return 1;
}
