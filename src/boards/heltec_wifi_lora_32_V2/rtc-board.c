/*!
 * \file      rtc-board.c
 *
 * \brief     Target board RTC timer and low power modes management
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
 *
 * \author    Marten Lootsma(TWTG) on behalf of Microchip/Atmel (c)2017
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "board-config.h"
#include "board.h"
#include "timer.h"
#include "systime.h"
#include "gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"
#include "rtc-board.h"






void RtcInit( void )
{

}

/**
* \brief Gets the absolute time value
* \retval Absolute time in ticks
*/
uint32_t HwTimerGetTime(void)
{
	int64_t tt = esp_timer_get_time();
	return (int32_t)(tt/1000);
}




uint32_t RtcMs2Tick( TimerTime_t milliseconds )
{
    return ( uint32_t )( milliseconds );
}

TimerTime_t RtcTick2Ms( uint32_t tick )
{
    uint32_t seconds = tick >> 10;

    tick = tick & 0x3FF;
    return ( ( seconds * 1000 ) + ( ( tick * 1000 ) >> 10 ) );
}

/*!
 * \brief a delay of delay ms by polling RTC
 *
 * \param[IN] delay in ms
 */
void RtcDelayMs( uint32_t delay )
{
	vTaskDelay(delay / portTICK_PERIOD_MS);
}







uint32_t RtcGetTimerValue( void )
{
    return ( uint32_t )HwTimerGetTime( );
}



uint32_t RtcGetCalendarTime( uint16_t *milliseconds )
{
    uint32_t ticks = 0;

    uint32_t calendarValue = HwTimerGetTime( );

    uint32_t seconds = ( uint32_t )calendarValue >> 10;

    ticks =  ( uint32_t )calendarValue & 0x3FF;

    *milliseconds = RtcTick2Ms( ticks );

    return seconds;
}

void RtcBkupWrite( uint32_t data0, uint32_t data1 )
{

}

void RtcBkupRead( uint32_t *data0, uint32_t *data1 )
{

}

void RtcProcess( void )
{
    // Not used on this platform.
}

TimerTime_t RtcTempCompensation( TimerTime_t period, float temperature )
{
    return period;
}




