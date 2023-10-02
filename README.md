# stm32-projects
Here are a few of the projects I've done on STM32 microcontrollers.
Sorry for poor naming, I still have lots to learn about this platform!

### Micro_pre1 
does a blink of an LED with adjustable time for being on and off using a little bit of assembly.
(May god never curse anyone with having such problems to solve)

### Project_1
is the code that can be used to implement a receiver that takes analog input through nRF24l01 transceiver
and converts the input to a digital siganl and uses it to control a PWM that can adjust the level of 
brightness on an LED. 

### asm_ex1
does pretty much the same thing as the Micro_pre1, with some clock adjustments using NOP in assembly.

### toggle_interrupt
takes an external interrupt from a pin and toggles an LED when interrupt function gets executed.

### Project_3 
does the rpm control process on a DC Motor, comments will lead you to understand this one better.
Note that this code has not been implemented and might need adjustments to work properly on a motor.
