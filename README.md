# UART-USART-in-STM32
Project Detail:

I. When the device boots, the HyperTerminal (HT) will output a list of choices:

Choose your option (1, 2, ..):
1. Student info
2. Basic operation
3. Simple led
4. Advance led
5. Audio
6. ...

II. Student info function:

1. Student info
ID: xxx
Full name: yyyy
ESC: return previous menu

III. Basic operation function:

2. Basic operation (a, b, ..):
a. Plus
b. Subtract
c. Multiple
d. Divide
e. Module
ESC: return previous menu

a. Plus function: choose one of two ways from below or create another

a. Plus
Operand 1: 5
Operand 2: 4
Result: 9
ESC: return previous menu

b. Functions Subtract, Muliple, Divide, Module: similar to Plus

IV. Simple led function: supports some command manipulation on Led

a. On Green: Lights up Green
b. Off Green: Turn off the Green light

V. Advance led function: This function is a combination of EXTI, SPI and Led

+ Control board HT plays the role of SPI master in communication.
+ Each time you click the User button, there will be 1 Led light. (See also Set led)
+ The LEDs turn clockwise or vice versa. (See also Set direction)
+ If exiting the Advance Led function (when pressing the ESC button), all LEDs are off.

4. Advance led (a, b, c):
a. Set led
b. Set direction
c. Start
ESC: return previous menu

a. Set led function: set which light will light first when performing the Start function

a. Set led (r, g, o, b):
ESC: return previous menu

b. Set direction function: lights turn clockwise (clockwise) or vice versa (anticlockwise)

b. Set direction (c, a):
ESC: return previous menu

c. Function Start: function is executing, pressing ESC will pause execution and keep the light status unchanged.

c. Running ...
ESC: return previous menu

VI. Audio function: support to play a piece of music (stored in Flash memory) in wave format.

5. Playing…
ESC: return previous menu 
