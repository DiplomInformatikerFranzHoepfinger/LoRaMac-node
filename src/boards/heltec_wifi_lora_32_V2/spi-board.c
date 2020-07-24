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



static spi_device_handle_t spi_lora = NULL;

/**
 * @brief Size of SPI transfer buffer
 */
#define SPI_MAX_TRANSFER_SIZE 4095

/**
 * @brief Transfer speed in Hz
 */
#define SPI_TRANSFER_SPEED 1*1000*1000

void SpiInit( Spi_t *obj, SpiId_t spiId, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{


	esp_err_t        ret;
	spi_bus_config_t buscfg =
	{
		.miso_io_num     = miso,
		.mosi_io_num     = mosi,
		.sclk_io_num     = sclk,
		.quadwp_io_num   = -1,
		.quadhd_io_num   = -1,
		.max_transfer_sz = SPI_MAX_TRANSFER_SIZE
	};

	spi_device_interface_config_t devcfg_lora =
	{
		.clock_speed_hz = SPI_TRANSFER_SPEED,
		.mode           = 0,
		.spics_io_num   = sclk,
		.queue_size     = 7,
		.pre_cb         = NULL,
	};

    CRITICAL_SECTION_BEGIN( );

    obj->SpiId = spiId;

    if( spiId == SPI_1 )
    {

    	ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    	ESP_ERROR_CHECK(ret);

    	ret = spi_bus_add_device(VSPI_HOST, &devcfg_lora, &spi_lora);
    	ESP_ERROR_CHECK(ret);
    }
    else
    {

    }


    CRITICAL_SECTION_END( );

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


void SpiInOut( Spi_t *obj, uint8_t cmd, uint8_t *buf, uint8_t len)
{
	spi_transaction_t spiTransaction;
    memset(buf, 0, len);
    memset(&spiTransaction, 0, sizeof(spiTransaction));
    spiTransaction.addr = cmd;
    spiTransaction.length = 8 * len;
    spiTransaction.rxlength = 8 * len;
    spiTransaction.tx_buffer = buf;
    spiTransaction.rx_buffer = buf;
    esp_err_t err = spi_device_transmit(spi_lora, &spiTransaction);
    ESP_ERROR_CHECK(err);
}

