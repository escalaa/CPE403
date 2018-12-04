#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// Holding the ADC values that come in FIFO
uint32_t ui32ADC0Value[4];

// Variables for calculating temperature from the sensor
volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;

void UARTIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
    char characters = UARTCharGet(UART0_BASE); // get the input
    UARTprintf("%c", characters); // echo back the character
    if(characters == 'R')
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Turn on Red LED
    }
    else if(characters == 'r')
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Turn off Red LED
    }
    else if(characters == 'G')
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on Green LED
    }
    else if(characters == 'g')
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Turn off Green LED
    }
    else if(characters == 'B')
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // Turn on Blue LED
    }
    else if(characters == 'b')
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Turn off Blue LED
    }
    else if(characters == 'T') // Display the Temperature
    {
        // Clear the flag
        ROM_ADCIntClear(ADC0_BASE, 2);

        // Trigger the ADC Conversion
        ROM_ADCProcessorTrigger(ADC0_BASE, 2);

        // Wait for the conversion to Complete
        while(!ROM_ADCIntStatus(ADC0_BASE, 2, false))
        {
        }

        // Read the value from the ADC Sample Sequencer 2 FIFO
        ROM_ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value);

        // Calculate the Average temperature of sensor data
        ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;

        // TEMP in C = 147.5 - (( 75 * (VREFP - VREFN) * ADCVALUE) / 4096)
        ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;

        // Convert C to F  => F = ( ( C * 9 ) + 160 ) / 5
        ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

        // print temperature
        UARTprintf("\r\nThis is current Temperature: %d degrees \r\n", ui32TempValueF);
    }
}
int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED

    // Enable ADC0 Peripheral
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Samples 32 times with 4 samples per time
    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 16);

    // Configure GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // Configure ADC Sequencer 2
    ROM_ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

    // Configuring all 4 steps of the ADC sequencer
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_TS);

    // Sample Temperature sensor & configure interrupt flag
    // Last conversion on sequencer 2
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

    // Enable ADC sequencer 2
    ROM_ADCSequenceEnable(ADC0_BASE, 2);



    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
    UARTStdioConfig(0, 115200, SysCtlClockGet());
    UARTprintf("Enter The Command:\r\n");
    UARTprintf("R: Turn on RED LED\r\n");
    UARTprintf("r: Turn off RED LED\r\n");
    UARTprintf("G: Turn on GREEN LED\r\n");
    UARTprintf("g: Turn off GREEN LED\r\n");
    UARTprintf("B: Turn on BLUE LED\r\n");
    UARTprintf("b: Turn off BLUE LED\r\n");
    UARTprintf("T: Display Temperature\r\n");
    while (1) //let interrupt handler do the UART echo function
    {
    }
}
