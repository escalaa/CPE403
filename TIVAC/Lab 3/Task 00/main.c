#include <stdint.h> // Variable definitions for C99 standard
#include <stdbool.h> // Boolean definitions for C99 standard
#include "inc/hw_memmap.h" // Macros defining memory map
#include "inc/hw_types.h" // Defines common types and macros
#include "driverlib/sysctl.h" // Macros defining System Control API
#include "driverlib/gpio.h" // Macros defining GPIO API

uint8_t ui8PinData=2;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
        SysCtlDelay(2000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
        SysCtlDelay(2000000);
        if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData = ui8PinData*2;}
    }
	return 0;
}
