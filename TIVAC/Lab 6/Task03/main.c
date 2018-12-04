#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

#define PWM_FREQUENCY 55 // 55Hz frequency base to control servo


int main(void)
{
    volatile uint32_t ui32Load;
    volatile uint32_t ui32PWMClock;
    volatile uint8_t ui8Adjust;     //adjust the position of duty cycle
    ui8Adjust = 900;                // initial 10%

    // CPU to run at 40 MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    // PWM clock at 625 kHz
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    //Enable PWM1 and GPIOF
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Configure PF1, PF2, PF3 as a PWM
    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);

    // PWM clock = SYSCLOCK / 64
    // Count = PWM clock / 55Hz
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    while(1)
    {
       int redLED;
       int blueLED;
       int greenLED;

       for(redLED = 900; redLED >= 100; redLED--)
       {
           ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, redLED * ui32Load /1000);
           ROM_SysCtlDelay(100000);
           for(blueLED = 900; blueLED >= 100; blueLED--)
           {
               ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blueLED * ui32Load /1000);
               ROM_SysCtlDelay(100000);
               for(greenLED = 900; greenLED >= 100; greenLED--)
               {
                   ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, greenLED * ui32Load /1000);
                   ROM_SysCtlDelay(100000);
               }
           }
       }
    }

}
