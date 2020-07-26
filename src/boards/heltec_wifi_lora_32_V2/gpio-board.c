/*!
 * \file      gpio-board.c
 *
 * \brief     Target board GPIO driver implementation
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
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "gpio-board.h"


#define TAG "--gpio-board--"

#define ESP_INTR_FLAG_DEFAULT 0

void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
    obj->pin = pin;

    if( pin == NC )
    {
        return;
    }


    switch(mode)
    {
    case PIN_INPUT:
        gpio_set_direction( obj->pin, GPIO_MODE_INPUT );
        break;
    case PIN_OUTPUT:
        gpio_set_direction( obj->pin, GPIO_MODE_OUTPUT );
    	break;
    case PIN_ALTERNATE_FCT:
    	break;
    case PIN_ANALOGIC:
    	break;
    }


    switch (config) {
	case PIN_PUSH_PULL:
		break;
	case PIN_OPEN_DRAIN:
		break;
	}

	switch (type) { // the pull up/down need to be set after the direction for correct working
	case PIN_NO_PULL:
		gpio_pullup_dis(obj->pin);
		gpio_pulldown_dis(obj->pin);
		break;
	case PIN_PULL_UP:
		gpio_pullup_en(obj->pin);
		gpio_pulldown_dis(obj->pin);
		break;
	case PIN_PULL_DOWN:
		gpio_pullup_dis(obj->pin);
		gpio_pulldown_en(obj->pin);
		break;
	}



    // Sets initial output value
    if( mode == PIN_OUTPUT )
    {
        GpioMcuWrite( obj, value );
    }
}

void GpioMcuSetContext( Gpio_t *obj, void* context )
{
    obj->Context = context;
}

void GpioInitIrq()
{
    ESP_LOGI(TAG, "gpio_install_isr_service");
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    ESP_LOGI(TAG, "GpioMcuSetInterrupt PIN: %i ", obj->pin);

    switch(irqMode)
    {
    case NO_IRQ:
        //change gpio intrrupt type for one pin
        gpio_set_intr_type(obj->pin, GPIO_PIN_INTR_DISABLE);
		break;
    case IRQ_RISING_EDGE:
        //change gpio intrrupt type for one pin
        gpio_set_intr_type(obj->pin, GPIO_PIN_INTR_POSEDGE);
    	break;
    case IRQ_FALLING_EDGE:
        //change gpio intrrupt type for one pin
        gpio_set_intr_type(obj->pin, GPIO_PIN_INTR_NEGEDGE);
    	break;
    case IRQ_RISING_FALLING_EDGE:
        //change gpio intrrupt type for one pin
        gpio_set_intr_type(obj->pin, GPIO_INTR_ANYEDGE);
    	break;
    }
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(obj->pin, irqHandler, (void*) obj->pin);
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    //remove isr handler for gpio number.
    gpio_isr_handler_remove(obj->pin);
}

void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{

    if( obj == NULL )
    {
        assert( 0 );
    }
    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return;
    }
    gpio_set_level( obj->pin, value );
}

void GpioMcuToggle( Gpio_t *obj )
{
    if( obj == NULL )
    {
        assert( 0 );
        while( 1 );
    }

    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return;
    }
    //ESP32 has no Tooggle Function.
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
    if( obj == NULL )
    {
        assert( 0 );
    }
    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return 0;
    }
    return ( uint32_t )gpio_get_level( obj->pin );
}
