/*!
 * \file      timer.c
 *
 * \brief     Timer objects and scheduling management implementation
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
#include "esp_timer.h"
#include "esp_log.h"
#include "utilities.h"
#include "board.h"
#include "rtc-board.h"
#include "timer.h"




#define TAG "timer"

void TimerInit(const esp_timer_create_args_t* create_args,
		TimerEvent_t* out_handle)
{
	out_handle->create_args = (esp_timer_create_args_t*)create_args;
	ESP_LOGI(TAG, "TimerInit %s", out_handle->create_args->name);
    ESP_ERROR_CHECK(esp_timer_create(create_args, &out_handle->esp_timer));
}


void TimerStart( TimerEvent_t *obj )
{
	ESP_LOGI(TAG, "TimerStart %s", obj->create_args->name);
	ESP_ERROR_CHECK(esp_timer_start_once(obj->esp_timer, obj->timeout_us));
}



void TimerStop( TimerEvent_t *obj )
{
	ets_printf("TimerStop %s\n", obj->create_args->name);
	esp_timer_stop(obj->esp_timer);
}



void TimerReset( TimerEvent_t *obj )
{
	ESP_LOGI(TAG, "TimerReset %s", obj->create_args->name);
    TimerStop( obj );
    TimerStart( obj );
}

void TimerSetValue( TimerEvent_t *obj, uint32_t timeout_ms )
{
	ESP_LOGI(TAG, "TimerSetValue %s , %i", obj->create_args->name, timeout_ms);
	obj->timeout_us = timeout_ms * 1000;
}

TimerTime_t TimerGetCurrentTime( void )
{
    uint32_t now = RtcGetTimerValue( );
    return  RtcTick2Ms( now );
}

TimerTime_t TimerGetElapsedTime( TimerTime_t past )
{
    if ( past == 0 )
    {
        return 0;
    }
    uint32_t nowInTicks = RtcGetTimerValue( );
    uint32_t pastInTicks = RtcMs2Tick( past );

    // Intentional wrap around. Works Ok if tick duration below 1ms
    return RtcTick2Ms( nowInTicks - pastInTicks );
}



TimerTime_t TimerTempCompensation( TimerTime_t period, float temperature )
{
    return RtcTempCompensation( period, temperature );
}

void TimerProcess( void )
{
    RtcProcess( );
}
