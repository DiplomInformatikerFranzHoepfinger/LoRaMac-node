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
#include <sys/time.h>
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


#define TAG "rtc-board"




/**
* \brief Gets the absolute time value
* \retval Absolute time in ticks
*/
struct timeval HwTimerGetTime(void)
{
	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);
	return tv_now;
}



void PrintTime(void)
{

	time_t now;
	char strftime_buf[64];
	struct tm timeinfo;

	time(&now);
	// Set timezone to China Standard Time
	setenv("TZ", "CST-8", 1);
	tzset();

	localtime_r(&now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

}


int64_t RtcMs2Tick( TimerTime_t milliseconds )
{
    return ( milliseconds * 1000 );
}

TimerTime_t RtcTick2Ms( int64_t tick )
{

    return ( tick / 1000 );
}









int64_t RtcGetTimerValue( void )
{
    struct timeval tv_now = HwTimerGetTime( );
    int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
    return time_us;
}



uint32_t RtcGetCalendarTime( uint16_t *milliseconds )
{
    struct timeval calendarValue = HwTimerGetTime( );

    uint32_t seconds = calendarValue.tv_sec;

    *milliseconds = calendarValue.tv_usec / 1000;

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




