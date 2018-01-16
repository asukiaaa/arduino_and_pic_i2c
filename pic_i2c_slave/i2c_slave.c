/*
 * File:   i2c_slave.c
 * Author: asuki
 *
 * Created on 2017/12/25, 10:11
 */

#include "i2c_slave.h"

void setupI2CSlave(short address) {
// start I2C(1) setup
    SSP1CON2 = 0b00000001;  // SEN is set to enable clock stretching
    SSP1CON3 = 0x00;
    SSP1ADD = address << 1; // 7-bit address is stored in the 7 MSB's of the SSP1ADD register**********
    SSP1STAT = 0x00;
    SSP1CON1 = 0b00110110;
//      B7: WCOL = 0;           // Clear write collisions
//      B6: SSPOV = 0;          // Clear receive overflow indicator
//      B5: SSPEN = 1;          // Enable SSP and configures SDA & SCL pins
//      B4: CKP = 1;            // Releases clock stretching
//      B<3:0>: SSPM = 0b0110;  // 7-bit addressing slave mode (Start and Stop bit interrupts disnabled)
                                // 000...0101   SPI mode
                                // 0110         I2C 7bit address
                                // 0111         I2C 10bit address
                                // 1000         I2C master
    /* Enable interrupts */
    SSP1IF  = 0; // Clear MSSP interrupt flag
    SSP1IE  = 1; // I2C interrupt enable
//end I2C(1) setup----------------------------------------------------------------

// Common Interrupt
    PEIE    = 1; // Enable Peripheral interrupts
    GIE     = 1; // Enable global interrupts
}

void I2CWrite(unsigned char data) {
    while(SSP1STATbits.BF);      // Wait while buffer is full
    do {
        SSP1CON1bits.WCOL = 0;   // Clear write collision flag
        SSP1BUF = data;
    } while (SSP1CON1bits.WCOL); // Do until write collision flag is clear
    if (SSP1CON2bits.SEN)
        SSP1CON1bits.CKP = 1;    // Release the SCL line
}

volatile unsigned char RXBufferIndex = 0;

#define STATE1 0b00001001 // 0x09 master write last was address
#define STATE2 0b00101001 // 0x29 master write last was data
#define STATE3 0b00001101 // 0x0d master read last was address
#define STATE4 0b00101100 // 0x2c master write last was data
#define STATE5 0b00101000 // 0x28

void countUpRXBufferIndex() {
    RXBufferIndex ++;
    if (RXBufferIndex >= RXBUFFER_SIZE)
        RXBufferIndex = 0;
}

void checkStateAndManageI2c() {
    static char DAStatus = 0;
    unsigned char i2cStatus, value;
    i2cStatus = SSP1STAT;
    i2cStatus = (i2cStatus & 0b00101101); // Mask out unimportant bits
    // _, _, D/A, _, S, R/W, _, BF
    switch (i2cStatus) {
        case STATE1:
            value = SSP1BUF;   // Read buffer, clear BF
            RXBufferIndex = 0; // Clear index
            DAStatus = 1;      // Next call is address inside memory

            if (SSP1CON1bits.SSPOV)
                SSP1CON1bits.SSPOV = 0; // Clear receive overflow indicator
            if (SSP1CON2bits.SEN)
                SSP1CON1bits.CKP = 1;   // Release the SCL line
            break;

        case STATE2:
            value = SSP1BUF; // Read buffer, clear BF

            if (DAStatus == 1) {
                RXBufferIndex = value;
                if (RXBufferIndex >= RXBUFFER_SIZE)
                    RXBufferIndex = 0;
                DAStatus = 2;
            } else {
                onI2CReceiveCallback(RXBufferIndex, value);
                countUpRXBufferIndex();
            }

            if (SSP1CON2bits.SEN)
                SSP1CON1bits.CKP = 1; // Release the SCL line
            break;

        case STATE3:
            value = SSP1BUF; // Dummy read

            setI2CWriteCharCallback(RXBufferIndex);
            I2CWrite(I2CWriteChar);
            countUpRXBufferIndex();
            break;

        case STATE4:
            setI2CWriteCharCallback(RXBufferIndex);
            I2CWrite(I2CWriteChar);
            countUpRXBufferIndex();
            break;

        case STATE5:
            break;
        default:
            if (SSP1CON2bits.SEN)
                SSP1CON1bits.CKP = 1; // Release the SCL line
            break;
    } // End switch (i2cStatus)
}

void interrupt I2Cinterrupt() {
    if (SSP1IF) {
        SSP1IF = 0; // Clear interrupt flag
        checkStateAndManageI2c();
    }
}
