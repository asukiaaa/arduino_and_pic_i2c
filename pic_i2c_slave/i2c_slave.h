#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define RXBUFFER_SIZE 255

void setupI2CSlave(short address);
void interrupt I2Cinterrupt();

int (*onI2CReceiveCallback)(unsigned char address, unsigned char data);
int (*setI2CWriteCharCallback)(unsigned char address);
unsigned char I2CWriteChar;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

