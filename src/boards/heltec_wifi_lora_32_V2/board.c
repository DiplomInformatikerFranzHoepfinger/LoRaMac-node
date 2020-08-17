/*!
 * \file      board.c
 *
 * \brief     Target board general functions implementation
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
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "utilities.h"
#include "gpio.h"
#include "adc.h"
#include "spi.h"
#include "i2c.h"
#include "timer.h"
#include "board-config.h"
#include "lpm-board.h"
#include "rtc-board.h"
#include "sx1276-board.h"
#include "board.h"


extern void GpioInitIrq();



/*!
 * Used to measure and calibrate the system wake-up time from STOP mode
 */
static void CalibrateSystemWakeupTime( void );

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;

/*!
 * Flag used to indicate if board is powered from the USB
 */
static bool UsbIsConnected = false;


void BoardCriticalSectionBegin( uint32_t *mask )
{
    //TODO
}

void BoardCriticalSectionEnd( uint32_t *mask )
{
    //TODO
}

void BoardInitPeriph( void )
{


    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

}

void BoardInitMcu( void )
{

	PrintTime();
    SpiInit( &SX1276.Spi, SPI_1, RADIO_MOSI, RADIO_MISO, RADIO_SCLK, RADIO_NSS );
    GpioInitIrq();
    SX1276IoInit( );

    if( McuInitialized == false )
    {
        McuInitialized = true;
        SX1276IoDbgInit( );
        SX1276IoTcxoInit( );
        if( GetBoardPowerSource( ) == BATTERY_POWER )
        {
            CalibrateSystemWakeupTime( );
        }
    }
}

void BoardResetMcu( void )
{
    CRITICAL_SECTION_BEGIN( );

    //Restart system
    //TODO
}

void BoardDeInitMcu( void )
{
    SpiDeInit( &SX1276.Spi );
    SX1276IoDeInit( );
}

uint32_t BoardGetRandomSeed( void )
{
	//TODO !!
    return 0;
}

void BoardGetUniqueId( uint8_t *id )
{


    uint8_t b[6];
    esp_efuse_mac_get_default(b);

    ESP_LOGI("BoardGetUniqueId","MAC = ## " MACSTR " ##", MAC2STR(b));


	//TODO get it from MAC Address !

    id[7] = b[5];
    id[6] = b[4];
    id[5] = b[3];
    id[4] = b[2];
    id[3] = b[1];
    id[2] = b[0];
    id[1] = 0xFE;
    id[0] = 0xCA;
}

uint16_t BoardBatteryMeasureVoltage( void )
{
    return 0;
}

uint32_t BoardGetBatteryVoltage( void )
{
    return 0;
}

uint8_t BoardGetBatteryLevel( void )
{
	//TODO adopt to ESP32 and Heltec.
    return 0;
}


void CalibrateSystemWakeupTime( void )
{


}



void SysTick_Handler( void )
{


}

uint8_t GetBoardPowerSource( void )
{
	//TODO !!
    if( UsbIsConnected == false )
    {
        return BATTERY_POWER;
    }
    else
    {
        return USB_POWER;
    }
}

/**
  * \brief Enters Low Power Stop Mode
  *
  * \note ARM exists the function when waking up
  */
void LpmEnterStopMode( void)
{
	//TODO
}

/*!
 * \brief Exists Low Power Stop Mode
 */
void LpmExitStopMode( void )
{
	//TODO
}

/*!
 * \brief Enters Low Power Sleep Mode
 *
 * \note ARM exits the function when waking up
 */
void LpmEnterSleepMode( void)
{
    //TODO
}

void BoardLowPowerHandler( void )
{
	vTaskDelay(10 / portTICK_PERIOD_MS);
}





#ifdef USE_FULL_ASSERT

#include <stdio.h>

/*
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 */
void assert_failed( uint8_t* file, uint32_t line )
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %lu\n", file, line) */

    printf( "Wrong parameters value: file %s on line %lu\n", ( const char* )file, line );
    /* Infinite loop */
    while( 1 )
    {
    }
}
#endif
