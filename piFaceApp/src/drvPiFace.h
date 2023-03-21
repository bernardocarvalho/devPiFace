/*
 * SPI
 *
 */

#include <stdint.h>

#ifndef PIFACE_C_INIT           //Do only once the first time this file is used
#define PIFACE_C_INIT

int piface_read_register(int fd, uint8_t address, uint8_t* value);
int piface_write_register(int fd, uint8_t address, uint8_t value);
int piface_open();

//----- SPI ACCESS SETUP -----
#define PIFACE_SPI_DEVICE                               0                       //0=CS0

#define PIFACE_CMD_WRITE                                0x40
#define PIFACE_CMD_READ                                 0x41

#define PIFACE_IODIRA                                   0x00    //I/O direction A
#define PIFACE_IODIRB                                   0x01    //I/O direction B
#define PIFACE_IOCON                                    0x0A    //I/O config
#define PIFACE_GPIOA                                    0x12    //Port A (PiFace uses as Output)
#define PIFACE_GPIOB                                    0x13    //Port B (PiFace uses as Input)
#define PIFACE_GPPUA                                    0x0C    //Port A pullups
#define PIFACE_GPPUB                                    0x0D    //Port B pullups

#endif

//  vim: syntax=c ts=4 sw=4 sts=4 sr et

