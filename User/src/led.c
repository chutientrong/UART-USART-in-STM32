/************************************************************************/
/*	History										Prepared/Updated by		*/
/*		+19/10/2012: initial version			cqkhanh					*/
/************************************************************************/

#include "led.h"

/************************************************************************/
/**
  * @brief	Initialize led
  * @param  None
  * @retval None
  */
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* enable GPIOD clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_RED | LED_GREEN | LED_ORANGE | LED_BLUE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);	
}

/************************************************************************/
/**
  * @brief	Turn on the led
  * @param  The led need be on
  * @retval None
  */
void led_on(uint16_t led)
{
	LED_PORT->ODR |= led;
}

/************************************************************************/
/**
  * @brief	Turn off the led
  * @param  The led need be off
  * @retval None
  */
void led_off(uint16_t led)
{
	LED_PORT->ODR &= (~led);
}

/************************************************************************/
/**
  * @brief	Toggle the led
  * @param  The led need be toggled
  * @retval None
  */
void led_toggle(uint16_t led)
{
	LED_PORT->ODR ^= led;
}

/************************************************************************/
