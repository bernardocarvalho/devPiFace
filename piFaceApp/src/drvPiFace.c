/*
*/

//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "drvPiFace.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const char *device = "/dev/spidev0.0";
//static const char *device = "/dev/spidev1.1";
//----- SET SPI MODE -----
//SPI_MODE_0 (0,0)  CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_1 (0,1)  CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
//SPI_MODE_2 (1,0)  CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
//SPI_MODE_3 (1,1)  CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)

static uint8_t mode = SPI_MODE_0;
static uint8_t bits = 8;
static uint32_t speed = 1000000; //Max speed PiFace 10MHz (1000000 gives 1MHz = 1uS per bit (measured))

int piface_open(){

    int ret = 0;
    int fd;
    fd = open(device, O_RDWR);
    if (fd < 0)
        return fd;

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        return ret;
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        return ret;
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        return ret;
    //§        pabort("can't set bits per word");
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        //        pabort("can't get bits per word");
        return ret;

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        return ret;
    ret = piface_write_register(fd, PIFACE_IOCON, 8);             //Enable hardware addressing
    if (ret == -1)
        // pabort("can't set max speed hz");
        return ret;

    piface_write_register(fd, PIFACE_IODIRA, 0);    //Set port A as an outputs
    piface_write_register(fd, PIFACE_GPIOA, 0xF0);  //Initialise all outputs to 0

    return fd;

}

int piface_read_register(int fd, uint8_t address, uint8_t* value) {
    int ret;
    uint8_t tx[] = {PIFACE_CMD_READ, 0, 0,};
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx),
        .delay_usecs = 0,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    tx[1] = address;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    *value = rx[2];
    return ret;
}

int piface_write_register(int fd, uint8_t address, uint8_t value) {
    int ret;
    uint8_t tx[] = {PIFACE_CMD_WRITE, 0, 0,};
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx),
        .delay_usecs = 0,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    tx[1] = address;
    tx[2] = value;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    return ret;
}

//  vim: syntax=c ts=4 sw=4 sts=4 sr et


