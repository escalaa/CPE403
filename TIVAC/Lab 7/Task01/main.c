#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


// Holding the ADC values that come in FIFO
uint32_t ui32ADC0Value[4];

// Variables for calculating temperature from the sensor
volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;

int main(void)
{
    // Set up the system clock at 40 MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

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

    /* TIMER1A Configurations */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()*0.5);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER1_BASE, TIMER_A);

    /* UART */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
    UARTStdioConfig(0, 115200, SysCtlClockGet());

    while(1)
    {
    }
}

void Timer1IntHandler(void)
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
    UARTprintf("This is current Temperature: %d degrees \r\n", ui32TempValueF);
    SysCtlDelay(1000000);
}
