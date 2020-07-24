/*!
 * \file      spi-board.c
 *
 * \brief     Target board SPI driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_sleep.h"


#include "utilities.h"
#include "board.h"
#include "gpio.h"
#include "spi-board.h"



static spi_device_handle_t SpiHandle[2];

void SpiInit( Spi_t *obj, SpiId_t spiId, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{
    CRITICAL_SECTION_BEGIN( );

    obj->SpiId = spiId;

    if( spiId == SPI_1 )
    {

    }
    else
    {

    }


    SpiFrequency( obj, 10000000 );



}

void SpiDeInit( Spi_t *obj )
{

}

void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{

}

void SpiFrequency( Spi_t *obj, uint32_t hz )
{

}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
    uint8_t rxData = 0;

    //TODO

    return( rxData );
}

