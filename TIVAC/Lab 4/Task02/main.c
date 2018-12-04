#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

int main(void)
{
    uint32_t ui32Period;
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); // setting the clock to 40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Enable Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // Enable Timer1
    /* Reconfigure SW2 */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    /* enable SW2 as an interrupt*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_0, GPIO_RISING_EDGE);
    IntEnable(INT_GPIOF);

    /*TIMER0 INTERRUPT*/
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    /* GPIO running at 2MHz
     * (Clock Speed / Desired Clock Speed) *( duty cycle)
     */
    ui32Period = (SysCtlClockGet()  / 2 )* 3 / 4;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
    while(1)
    {
    }
}
void Timer1DelayIntHandler(void)
{
    TimerDisable(TIMER1_BASE, TIMER_A);            // Turnoff Timer1A
    TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT); // Clear Timer1A
    IntDisable(INT_TIMER1A);                       // Turn off interrupt Timer1A
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);  // Turn off LED blue
    IntEnable(INT_GPIOF);                          // Turn on GPIO Port F interrupt
    IntEnable(INT_TIMER0A);                        // Turn on interrupt Timer0A
}
void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
   else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
}
void PortFPin0IntHandler(void)
{
    IntDisable(INT_GPIOF);                                    // Disable Port F interrupt
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0);            // clear GPIO Interrupt
    IntDisable(INT_TIMER0A);                                  // Disable Timer0A interrupt
    IntEnable(INT_TIMER1A);                                   // Enable Timer1A interrupt
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()*1.5); // Set Delay to 1.5 Seconds
    IntEnable(INT_TIMER1A);                                   // Turn on interrupt Timer1A
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);                        // Turn on Timer1A
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);             // Turn on LED BLUE
}
