/*
 * File:   main.c
 * Author: asuki
 *
 * Created on 2017/12/31, 22:25
 */

#include <xc.h>
#include <pic16f1829.h>
#include <stdint.h>
#include "i2c_slave.h"
#define I2C_ADDRESS 0x10
#define _XTAL_FREQ 16000000
#define LED_ON 1
#define LED_OFF 0
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config CP = OFF
#pragma config FOSC = INTOSC // Use internal oscillator

unsigned char r = 0, g = 0, b = 0;

void ledR(int ledState) {
    if (ledState == LED_OFF) {
        r = 0;
        PORTCbits.RC5 = 0;
    } else {
        r = 1;
        PORTCbits.RC5 = 1;
    }
}
void ledG(int ledState) {
    if (ledState == LED_OFF) {
        g = 0;
        PORTCbits.RC3 = 0;
    } else {
        g = 1;
        PORTCbits.RC3 = 1;
    }
}

void ledB(int ledState) {
    if (ledState == LED_OFF) {
        b = 0;
        PORTCbits.RC6 = 0;
    } else {
        b = 1;
        PORTCbits.RC6 = 1;
    }
}

void ledRGB(unsigned char data) {
    unsigned char portC = 0;
    // red
    if ((data & 0b0100) != 0) {
        portC = portC | 0b00100000;
    }
    // green
    if ((data & 0b0010) != 0) {
        portC = portC | 0b0001000;
    }
    // blue
    if ((data & 0b0001) != 0) {
        portC = portC | 0b01000000;
    }
    PORTC = portC;
}

void onI2CReceive(unsigned char address, unsigned char data) {
    switch (address) {
        case 0x00:
            ledRGB(data);
            break;
        case 0x01:
            ledR(data);
            break;
        case 0x02:
            ledG(data);
            break;
        case 0x03:
            ledB(data);
            break;
        default:
            break;
    }
}

void setI2CWriteChar(unsigned char address) {
    switch (address) {
        case 0x00:
        {
            unsigned char data = 0;
            if (r != 0) { data = data | 0b0100; }
            if (g != 0) { data = data | 0b0010; }
            if (b != 0) { data = data | 0b0001; }
            I2CWriteChar = data;
            return;
        }
        case 0x01:
            I2CWriteChar = r;
            return;
        case 0x02:
            I2CWriteChar = g;
            return;
        case 0x03:
            I2CWriteChar = b;
            return;
        default:
            I2CWriteChar = 0;
            return;
    }
}

void main(void) {
    OSCCONbits.IRCF = 0b1111; // 16MHz
    OSCCONbits.SCS = 0b11; // Use internal oscillator as system clock
    TRISC = 0x00;
    PORTC = 0x00;

    // Set callbacks
    onI2CReceiveCallback = onI2CReceive;
    setI2CWriteCharCallback = setI2CWriteChar;

    // Start as I2C slave
    setupI2CSlave(I2C_ADDRESS);

    while(1) {}
}
