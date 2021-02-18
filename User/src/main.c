#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "uart.h"
#include "lcd.h"
#include "queue.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "queueInt.h"
#include "main.h"
#include "math.h"
#include "stdint.h"
#include "stdbool.h"

#include "delay.h"
#include "rtc.h"
#include <stdio.h>	
// Define Macros
#define STR_MAX_LENGTH 100

#define RED GPIO_Pin_12						   // Green LED connects to PD12
#define GREEN GPIO_Pin_13					   // Orange LED connects to PD13
#define ORANGE GPIO_Pin_14					   // Red LED connects to PD14
#define BLUE GPIO_Pin_15					   // Blue LED connects to PD15
#define ALL_LEDS (RED | GREEN | ORANGE | BLUE) // all leds
#define LEDnn 4
#define LEDS_GPIO_PORT (GPIOD) // LEDs connect to Port D

/* User Button. */
#define USER_BUTTON GPIO_Pin_0	 // User Button connects to PA0
#define BUTTON_GPIO_PORT (GPIOA) // User Button connects to Port A

/* The array stores the led order used to switch them on and off. */
uint16_t leds[LEDnn] = {RED, GREEN, ORANGE, BLUE};

// Global variables
volatile char *gp_menu = "Choose your option(1,2,3,...): \n\r1. Studient info \n\r2. Basic operation \n\r3. Simple led \n\r4. Advance led \n\r5. Audio \n\r6. Timer \n\r ";
volatile char *gp_error = "Error. Please re-Enter \n\r";
volatile char *gp_visible_char_str = "Visible char";
volatile char *gp_invisible_char_str = "Invisible char";
volatile char *gp_new_line = "\r\n";
volatile char gp_char_input = 0;
volatile char gp_char_int = 0;

volatile char *gp_Student_Info = "1.Student info\n\r";
volatile char *gp_ID = "ID: 16521833\n\r";
volatile char *gp_fullname = "Full name: Chu Tien Trong\n\r";
volatile char *gp_ESC = "ESC: return previous menu\n\r";

volatile char *gp_Basic_Operation = "2.Basic operation(a,b,..):\n\r";
volatile char *gp_plus = "a. Plus\n\r";
volatile char *gp_subtract = "b. Subtract\n\r";
volatile char *gp_multiple = "c. Multiple\n\r";
volatile char *gp_divide = "d. Divide\n\r";
volatile char *gp_module = "e. Module\n\r";
volatile char *gp_operand1 = "Operation 1: \n\r";
volatile char *gp_operand2 = "Operation 2: \n\r";
volatile char *gp_result = "Result: \n\r";

volatile char *gp_Simple_Led = "3. Simple Led(a,b): \n\r";
volatile char *gp_on_green = "a. On Green \n\r";
volatile char *gp_led_on_green = "On Green \n\r";
volatile char *gp_off_green = "b. Off Green \n\r";
volatile char *gp_led_off_green = "Off Green \n\r";

volatile char *gp_Advance_Led = "4. Advance led(a,b,..):\n\r";
volatile char *gp_set_led = "a. Set led\n\r";
volatile char *gp_color = "a. Set led(r,g,o,b):\n\r";
volatile char *gp_set_derection = "b. Set derection\n\r";
volatile char *gp_c_a_wise = "b. Set direction (1, 2):\n\r";
volatile char *gp_c_wise = "1. Clockwise\n\r";
volatile char *gp_a_wise = "2. AntiClockwise\n\r";

volatile char *gp_c_wise_ok = "Clockwise OK!\n\r";
volatile char *gp_a_wise_ok = "AntiClockwise OK!\n\r";

volatile char *gp_start = "c. Start\n\r";
volatile char *gp_running = "c. Running...\n\r";

volatile char *gp_Audio = "5. Audio \n\r";
volatile char *gp_playing = "5. Playing... \n\r";

volatile char *gp_Timer = "6. RTC \n\r";

volatile int LEDToggle = 0;

int number1 = 0;
int number2 = 0;
char x;
char y;
volatile char x1[10];
volatile char y1[10];
int SumP = 0;
char Tong[50];
int SumS = 0;
char Hieu[50];
int SumM = 0;
char Tich[50];
int SumD = 0;
char Thuong[50];
float SumML = 0;
char Module[50];
int i = 0;
int j = 0;
int e = 0;
int i2 = 0;
int j2 = 0;
int e2 = 0;
char N1[10];
char N2[10];
int il = 0;
char led[100];
int count;
bool check;
char result[50]; 

const unsigned char month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

char hours[50];

char hour;
volatile uint32_t minute;
volatile uint32_t second;
volatile char time[10];
volatile char date[10];

RTC_InitTypeDef RTC_InitStructure;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
 
unsigned int update_time;

uint8_t iled;
uint8_t nstate = 0;
uint8_t pstate = 0;
uint8_t u8_status;

uint8_t aRxBuffer1;
uint8_t test_value[3];

// Declare data structure
queue_t g_queue_send;
queue_t g_queue_receive;
iqueue_t ig_queue_send;
iqueue_t ig_queue_receive;


int fputc(int ch, FILE *f)    
{
    USART_SendData(USART3, (u8)ch);   
    return ch;
}
void Clear()
{
	memset((void *)Tong, '\0', 50);
	memset((void *)x1, '\0', 10);
	memset((void *)y1, '\0', 10);
	number1 = 0;
	number2 = 0;
	SumP = 0;
	SumS = 0;
	memset(Hieu, '\0', 50);
	SumM = 0;
	memset(Tich, '\0', 50);
	SumD = 0;
	memset(Module, '\0', 50);
	SumML = 0;
	memset(Thuong, '\0', 50);
	i = 0;
	j = 0;
	e = 0;
	i2 = 0;
	j2 = 0;
	e2 = 0;
	il = 0;
}
void StudentInfo(void)
{
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_Student_Info, strlen((char *)gp_Student_Info));
	queue_push_string(&g_queue_send, (char *)gp_ID, strlen((char *)gp_ID));
	queue_push_string(&g_queue_send, (char *)gp_fullname, strlen((char *)gp_fullname));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
			else
			{
				LCD_DisplayStringLine(0, (uint8_t *)gp_invisible_char_str);
				// Output to HT the message "Don't support"
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_error, strlen((char *)gp_error));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}
void BO_Plus(void)
{
	Clear();
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
	queue_push_string(&g_queue_send, (char *)gp_operand1, strlen((char *)gp_operand1));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				x1[i] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_operand2, strlen((char *)gp_operand2));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				y1[i2] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i2++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_result, strlen((char *)gp_result));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	//	Tong = var1 + var2;
	for (e = 0; e < strlen((char *)x1); e++)
	{
		j = (strlen((char *)x1) - e - 1);
		number1 = number1 + (x1[e] - '0') * pow(10, j);
	}
	for (e2 = 0; e2 < strlen((char *)y1); e2++)
	{
		j2 = (strlen((char *)y1) - e2 - 1);
		number2 = number2 + (y1[e2] - '0') * pow(10, j2);
	}
	SumP = number1 + number2;
	sprintf(Tong, "%d", SumP);
	queue_push_string(&g_queue_send, (char *)Tong, strlen((char *)Tong));
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Basic_Operation, strlen((char *)gp_Basic_Operation));
				queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
				queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
				queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
				queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
				queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void BO_Subtract(void)
{
	Clear();
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
	queue_push_string(&g_queue_send, (char *)gp_operand1, strlen((char *)gp_operand1));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				x1[i] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_operand2, strlen((char *)gp_operand2));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				y1[i2] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i2++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_result, strlen((char *)gp_result));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	for (e = 0; e < strlen((char *)x1); e++)
	{
		j = (strlen((char *)x1) - e - 1);
		number1 = number1 + (x1[e] - '0') * pow(10, j);
	}
	for (e2 = 0; e2 < strlen((char *)y1); e2++)
	{
		j2 = (strlen((char *)y1) - e2 - 1);
		number2 = number2 + (y1[e2] - '0') * pow(10, j2);
	}
	SumS = number1 - number2;
	sprintf(Hieu, "%d", SumS);
	queue_push_string(&g_queue_send, (char *)Hieu, strlen((char *)Hieu));
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Basic_Operation, strlen((char *)gp_Basic_Operation));
				queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
				queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
				queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
				queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
				queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void BO_Multiple(void)
{
	Clear();
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
	queue_push_string(&g_queue_send, (char *)gp_operand1, strlen((char *)gp_operand1));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				x1[i] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_operand2, strlen((char *)gp_operand2));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				y1[i2] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i2++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_result, strlen((char *)gp_result));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	for (e = 0; e < strlen((char *)x1); e++)
	{
		j = (strlen((char *)x1) - e - 1);
		number1 = number1 + (x1[e] - '0') * pow(10, j);
	}
	for (e2 = 0; e2 < strlen((char *)y1); e2++)
	{
		j2 = (strlen((char *)y1) - e2 - 1);
		number2 = number2 + (y1[e2] - '0') * pow(10, j2);
	}
	SumM = number1 * number2;
	sprintf(Tich, "%d", SumM);
	queue_push_string(&g_queue_send, (char *)Tich, strlen((char *)Tich));
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Basic_Operation, strlen((char *)gp_Basic_Operation));
				queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
				queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
				queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
				queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
				queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void BO_Divide(void)
{
	Clear();
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
	queue_push_string(&g_queue_send, (char *)gp_operand1, strlen((char *)gp_operand1));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				x1[i] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_operand2, strlen((char *)gp_operand2));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				y1[i2] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i2++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_result, strlen((char *)gp_result));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	for (e = 0; e < strlen((char *)x1); e++)
	{
		j = (strlen((char *)x1) - e - 1);
		number1 = number1 + (x1[e] - '0') * pow(10, j);
	}
	for (e2 = 0; e2 < strlen((char *)y1); e2++)
	{
		j2 = (strlen((char *)y1) - e2 - 1);
		number2 = number2 + (y1[e2] - '0') * pow(10, j2);
	}
	SumD = number1 / number2;
	sprintf(Thuong, "%d", SumD);
	queue_push_string(&g_queue_send, (char *)Thuong, strlen((char *)Thuong));
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Basic_Operation, strlen((char *)gp_Basic_Operation));
				queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
				queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
				queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
				queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
				queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void BO_Module(void)
{
	Clear();
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
	queue_push_string(&g_queue_send, (char *)gp_operand1, strlen((char *)gp_operand1));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				x1[i] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_operand2, strlen((char *)gp_operand2));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input <= 57 && gp_char_input >= 48)
			{
				y1[i2] = gp_char_input;
				queue_push(&g_queue_send, gp_char_input);
				i2++;
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			}
			if (gp_char_input == 13)
			{
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_result, strlen((char *)gp_result));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	for (e = 0; e < strlen((char *)x1); e++)
	{
		j = (strlen((char *)x1) - e - 1);
		number1 = number1 + (x1[e] - '0') * pow(10, j);
	}
	for (e2 = 0; e2 < strlen((char *)y1); e2++)
	{
		j2 = (strlen((char *)y1) - e2 - 1);
		number2 = number2 + (y1[e2] - '0') * pow(10, j2);
	}
	SumML = (float)number1 / number2;
	sprintf(Module, "%f", SumML);
	queue_push_string(&g_queue_send, (char *)Module, strlen((char *)Module));
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Basic_Operation, strlen((char *)gp_Basic_Operation));
				queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
				queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
				queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
				queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
				queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void BasicOperation(void)
{
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_Basic_Operation, strlen((char *)gp_Basic_Operation));
	queue_push_string(&g_queue_send, (char *)gp_plus, strlen((char *)gp_plus));
	queue_push_string(&g_queue_send, (char *)gp_subtract, strlen((char *)gp_subtract));
	queue_push_string(&g_queue_send, (char *)gp_multiple, strlen((char *)gp_multiple));
	queue_push_string(&g_queue_send, (char *)gp_divide, strlen((char *)gp_divide));
	queue_push_string(&g_queue_send, (char *)gp_module, strlen((char *)gp_module));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
			switch (gp_char_input)
			{
			case 97:
				BO_Plus();
				break;
			case 98:
				BO_Subtract();
				break;
			case 99:
				BO_Multiple();
				break;
			case 100:
				BO_Divide();
				break;
			case 101:
				BO_Module();
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void setup_leds(void)
{
	/* Structure storing the information of GPIO Port D. */
	static GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable the GPIOD peripheral clock. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* Pin numbers of LEDs are mapped to bits number. */
	GPIO_InitStructure.GPIO_Pin = ALL_LEDS;
	/* Pins in output mode. */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	/* Clock speed rate for the selected pins. */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	/* Operating output type for the selected pins. */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Operating Pull-up/Pull down for the selected pins. */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/* Write this data into memory at the address
     * mapped to GPIO device port D, where the led pins
     * are connected */
	GPIO_Init(LEDS_GPIO_PORT, &GPIO_InitStructure);
}

/* Initialize the GPIO port A for input User Button. */
void setup_button(void)
{
	/* Structure storing the information of GPIO Port A. */
	static GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable the GPIOA peripheral clock. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Pin number of User Button is mapped to a bit number. */
	GPIO_InitStructure.GPIO_Pin = USER_BUTTON;
	/* Pin in input mode. */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	/* Clock speed rate for the selected pins. */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	/* Operating output type for the selected pins. */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Operating Pull-up/Pull down for the selected pins. */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	/* Write this data into memory at the address
     * mapped to GPIO device port A, where the led pins
     * are connected */
	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

/* Get the status of User Button.
 * 0: Not pressed.
 * 1: Pressed.
 */
uint8_t read_button(void)
{
	return GPIO_ReadInputDataBit(BUTTON_GPIO_PORT, USER_BUTTON);
}

/* Turn all leds on and off 4 times. */
void flash_all_leds(void)
{
	int il;
	for (il = 0; il < 4; il++)
	{
		/* Turn on all user leds */
		GPIO_SetBits(LEDS_GPIO_PORT, ALL_LEDS);
		/* Wait a short time */
		Delay_ms(100);
		/* Turn off all leds */
		GPIO_ResetBits(LEDS_GPIO_PORT, ALL_LEDS);
		/* Wait again before looping */
		Delay_ms(100);
	}
}

void SimpleLed(void)
{
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_Simple_Led, strlen((char *)gp_Simple_Led));
	queue_push_string(&g_queue_send, (char *)gp_on_green, strlen((char *)gp_on_green));
	queue_push_string(&g_queue_send, (char *)gp_off_green, strlen((char *)gp_off_green));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
			switch (gp_char_input)
			{
			case 97:
				queue_push_string(&g_queue_send, (char *)gp_led_on_green, strlen((char *)gp_led_on_green));
				GPIO_SetBits(LEDS_GPIO_PORT, GREEN);
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
				break;
			case 98:
				queue_push_string(&g_queue_send, (char *)gp_led_off_green, strlen((char *)gp_led_off_green));
				GPIO_ResetBits(LEDS_GPIO_PORT, GREEN);
				USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
				break;
			}
		}
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void SetLed(void)
{
	iled=0;
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_color, strlen((char *)gp_color));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	/* Setup input / output for User Button and LEDs. */
	setup_leds();
	setup_button();
	/* Wellcome LEDs. */
	GPIO_ResetBits(LEDS_GPIO_PORT, ALL_LEDS);
	flash_all_leds();
	GPIO_SetBits(LEDS_GPIO_PORT, leds[iled]);
	while (1)
	{
		nstate = read_button();
		/* Check the User Button is pressed or not. */
		if ((nstate == 1) && (pstate == 0))
		{
			/* If the User Button is pressed. */
			/* Turn off all LEDS. */
			GPIO_ResetBits(LEDS_GPIO_PORT, ALL_LEDS);
			/* Choose next LED and turn it on. */
			iled = (iled + 1) % LEDnn;
			GPIO_SetBits(LEDS_GPIO_PORT, leds[iled]);
		}
		/* Save the current state by previous state. */
		pstate = nstate;
		/* Avoid button ocsillation. */
		Delay_ms(1000);
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Advance_Led, strlen((char *)gp_Advance_Led));
				queue_push_string(&g_queue_send, (char *)gp_set_led, strlen((char *)gp_set_led));
				queue_push_string(&g_queue_send, (char *)gp_set_derection, strlen((char *)gp_set_derection));
				queue_push_string(&g_queue_send, (char *)gp_start, strlen((char *)gp_start));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void Clockwise(void)
{
	il = 1;
	count = 0;
	GPIO_SetBits(LEDS_GPIO_PORT, iled);
	while (1)
	{
		GPIO_ResetBits(LEDS_GPIO_PORT, ALL_LEDS);
		Delay_ms(100);
		GPIO_SetBits(LEDS_GPIO_PORT, leds[iled]);
		iled += il;
		if (iled == 3)
		{
			il = -3;
		}
		if (iled == 0)
		{
			il = 1;
		}
				Delay_ms(500);
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				break;
			}
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		}
	}
}
void AntiClockwise(void)
{
	il = 1;
	count = 0;
	GPIO_SetBits(LEDS_GPIO_PORT, iled);
	while (1)
	{
		GPIO_ResetBits(LEDS_GPIO_PORT, ALL_LEDS);
		Delay_ms(100);
		GPIO_SetBits(LEDS_GPIO_PORT, leds[iled]);
		iled -= il;
		if (iled == 3)
		{
			il = 1;
		}
		if (iled == 0)
		{
			il = -3;
		}
//		delay(LED_LONG);
		Delay_ms(500);
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
					break;
			}
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		}
	}
}

void SetDerection(void)
{
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	il = 0;
	queue_push_string(&g_queue_send, (char *)gp_c_a_wise, strlen((char *)gp_c_a_wise));
	queue_push_string(&g_queue_send, (char *)gp_c_wise, strlen((char *)gp_c_wise));
	queue_push_string(&g_queue_send, (char *)gp_a_wise, strlen((char *)gp_a_wise));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	setup_leds();
	setup_button();
	//flash_all_leds();
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);

			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Advance_Led, strlen((char *)gp_Advance_Led));
				queue_push_string(&g_queue_send, (char *)gp_set_led, strlen((char *)gp_set_led));
				queue_push_string(&g_queue_send, (char *)gp_set_derection, strlen((char *)gp_set_derection));
				queue_push_string(&g_queue_send, (char *)gp_start, strlen((char *)gp_start));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		gp_char_int = gp_char_input;
			switch (gp_char_input)
			{
			case 49:
				queue_push_string(&g_queue_send, (char *)gp_c_wise_ok, strlen((char *)gp_c_wise_ok));
				check=true;
				break;
			case 50:
				queue_push_string(&g_queue_send, (char *)gp_a_wise_ok, strlen((char *)gp_a_wise_ok));
				check=false;
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void Start(void)
{
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	queue_push_string(&g_queue_send, (char *)gp_running, strlen((char *)gp_running));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

	if (check==true)
	{
		Clockwise();
	}
	if (check==false)
	{
		AntiClockwise();
	}
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				iled=0;

//				GPIO_ResetBits(LEDS_GPIO_PORT, ALL_LEDS);
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);

				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_Advance_Led, strlen((char *)gp_Advance_Led));
				queue_push_string(&g_queue_send, (char *)gp_set_led, strlen((char *)gp_set_led));
				queue_push_string(&g_queue_send, (char *)gp_set_derection, strlen((char *)gp_set_derection));
				queue_push_string(&g_queue_send, (char *)gp_start, strlen((char *)gp_start));
				queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void AdvanceLed(void)
{
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_Advance_Led, strlen((char *)gp_Advance_Led));
	queue_push_string(&g_queue_send, (char *)gp_set_led, strlen((char *)gp_set_led));
	queue_push_string(&g_queue_send, (char *)gp_set_derection, strlen((char *)gp_set_derection));
	queue_push_string(&g_queue_send, (char *)gp_start, strlen((char *)gp_start));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
			switch (gp_char_input)
			{
			case 97:
				SetLed();
				break;
			case 98:
				SetDerection();
				break;
			case 99:
				Start();
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

void Audio(void)
{
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_Audio, strlen((char *)gp_Audio));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	while (1)
	{
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}


/*
//void RTC_Config(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
//	PWR_BackupAccessCmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//	RCC_RTCCLKCmd(ENABLE);
//	RTC_WaitForSynchro();

//	if (RTC_ReadBackupRegister (RTC_BKP_DR1) != 0x32F2) // a variable to see
//	//if RTC initialization is 
//	RTC_WriteProtectionCmd (DISABLE);

//	RTC_EnterInitMode();
//	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
//	RTC_InitStructure.RTC_AsynchPrediv = 127;
//	RTC_InitStructure.RTC_SynchPrediv = 255;
//	RTC_Init(&RTC_InitStructure);

//	RTC_TimeStructure.RTC_Seconds = 0x00;
//	RTC_TimeStructure.RTC_Minutes = 0x00;
//	RTC_TimeStructure.RTC_Hours = 0x00;
//	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
//	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);

//	RTC_DateStructure.RTC_Date = 30;
//	RTC_DateStructure.RTC_Month = 5;
//	RTC_DateStructure.RTC_WeekDay= RTC_Weekday_Thursday;
//	RTC_DateStructure.RTC_Year = 12;
//	RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);

//	RTC_ExitInitMode();
//	RTC_WriteBackupRegister(RTC_BKP_DR1, 0x32F2);
//	RTC_WriteProtectionCmd(ENABLE);
//	RTC_WriteBackupRegister(RTC_BKP_DR1, 0x32F2); 
//	PWR_BackupAccessCmd(DISABLE);
//}
*/
void RTC_Set()
{	
       RTC_DateTypeDef RTC_DateStructure;
        RTC_TimeTypeDef RTC_TimeStructure;
        RTC_InitTypeDef RTC_InitStructure;
        RTC_AlarmTypeDef RTC_AlarmStructure;
  uint8_t  uwSynchPrediv = 0xFF;
  uint8_t  uwAsynchPrediv = 0x7F;
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);


  /* Allow access to RTC */
   PWR_BackupAccessCmd(ENABLE);

  RCC_LSEConfig(RCC_LSE_ON);


  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/


  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);


  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);

  /* Set the alarm 05h:20min:30s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x05;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x20;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x05;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

  /* Configure the RTC Alarm A register */
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

  /* Enable RTC Alarm A Interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);

  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

  RTC_ClearFlag(RTC_FLAG_ALRAF);

  RTC_DateStructure.RTC_Year = 15;
  RTC_DateStructure.RTC_Month = RTC_Month_June;
  RTC_DateStructure.RTC_Date = 26;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 5;
  RTC_TimeStructure.RTC_Minutes = 20;
  RTC_TimeStructure.RTC_Seconds = 10;

  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
}
void Timer(void)
{
	queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
	queue_push_string(&g_queue_send, (char *)gp_Timer, strlen((char *)gp_Timer));
	queue_push_string(&g_queue_send, (char *)gp_ESC, strlen((char *)gp_ESC));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

	while (1)
	{
  /* USER CODE BEGIN 3 */

//		RTC_ReadBackupRegister(RTC_BKP_DR1);
//		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//		hours[0]=(RTC_TimeStructure.RTC_Hours/16)+48;
//		hours[1]=(RTC_TimeStructure.RTC_Hours%16)+48;

//		hours[2]=(RTC_TimeStructure.RTC_Minutes/16)+48;
//		hours[3]=(RTC_TimeStructure.RTC_Minutes%16)+48;
//		
//		hours[4]=(RTC_TimeStructure.RTC_Seconds/16)+48;
//		hours[5]=(RTC_TimeStructure.RTC_Seconds%16)+48;
//		
		
		RTC_TimeTypeDef RTC_Time_struct;
		uint8_t h,m,s;
		char str[500];

		RTC_GetTime(RTC_Format_BIN, &RTC_Time_struct);
		//show in display
		h=RTC_Time_struct.RTC_Hours;
		m=RTC_Time_struct.RTC_Minutes;
		s=RTC_Time_struct.RTC_Seconds;
		sprintf(str,"H=%d,M=%d,S=%d",h,m,s);
		queue_push_string(&g_queue_send, (char *)str, strlen((char *)str));
//		LCD_DisplayStringLine(2,(uint8_t*)hours);
		queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
//		Delay_ms(100);
		Delay_ms(1000);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			if (gp_char_input == 27)
			{
				// Output to LCD that it is esc
				LCD_Clear(LCD_COLOR_WHITE);
				// Output to LCD that it is visible character
				LCD_DisplayStringLine(0, (uint8_t *)gp_visible_char_str);
				// LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
				queue_push_string(&g_queue_send, (char *)gp_new_line, strlen((char *)gp_new_line));
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}

int main(void)
{
	// Initialize LCD
	lcd_init();
	Delay_Init();
	My_RTC_Init();
	// Initialize UART
	uart3_init_interrupt();
//	RTC_Config();
	// Initialize queue
	queue_init(&g_queue_send);
	queue_init(&g_queue_receive);
	// Push " Input your chracter: " to queue and then ignite sending interrupt
	queue_push_string(&g_queue_send, (char *)gp_menu, strlen((char *)gp_menu));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	
	while (1)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		if (!queue_is_empty(&g_queue_receive))
		{
			gp_char_input = queue_pop(&g_queue_receive);
			switch (gp_char_input)
			{
			case 49:
				StudentInfo();
				break;
			case 50:
				BasicOperation();
				break;
			case 51:
				SimpleLed();
				break;
			case 52:
				AdvanceLed();
				break;
			case 53:
				Audio();
				break;
			case 54:
				Timer();
				break;
			}
		}
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}
//--------------------------INTERRUPT HANDLER----------------------------
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_TXE) == SET)
	{
		// Send characters until queue is empty
		if (!queue_is_empty(&g_queue_send))
		{
			USART_SendData(USART3, queue_pop(&g_queue_send));
		}
		else
		{
			test_value[0] = aRxBuffer1;
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		}
	}

	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		queue_push(&g_queue_receive, (char)USART_ReceiveData(USART3));
	}
}

/* End of Life */
