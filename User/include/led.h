/************************************************************************/
/*	History																				Prepared/Updated by   */
/*		+19/10/2012: initial version										cqkhanh						*/
/************************************************************************/

#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
#define LED_PORT		GPIOD
#define LED_RED			GPIO_Pin_12
#define LED_GREEN		GPIO_Pin_13
#define LED_ORANGE	GPIO_Pin_14
#define LED_BLUE		GPIO_Pin_15
 
/* extern function */
extern void led_init(void);
extern void led_on(uint16_t led);
extern void led_off(uint16_t led);
extern void led_toggle(uint16_t led);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */
/************************************************************************/
