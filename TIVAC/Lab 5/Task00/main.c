#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"

#ifdef DEBUG
void__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

int main(void)
{
    // Holding the ADC values that come in FIFO
    uint32_t ui32ADC0Value[4];

    // Variables for calculating temperature from the sensor
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    // Set up the system clock at 40 MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    // Enable ADC0 Peripheral
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    // Configure ADC Sequencer
    ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

    // Configuring all 4 steps of the ADC sequencer
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);

    // Sample Temperature sensor & configure interrupt flag
    // Las conversion on sequencer 1
    ROM_ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

    // Enable ADC sequencer 1
    ROM_ADCSequenceEnable(ADC0_BASE, 1);

    while(1)
    {
        // Clear the flag
        ROM_ADCIntClear(ADC0_BASE, 1);

        // Trigger the ADC Conversion
        ROM_ADCProcessorTrigger(ADC0_BASE, 1);

        // Wait for the conversion to Complete
        while(!ROM_ADCIntStatus(ADC0_BASE, 1, false))
        {
        }

        // Read the value from the ADC Sample Sequencer 1 FIFO
        ROM_ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);

        // Calculate the Average temperature of sensor data
        ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;

        // TEMP in C = 147.5 - (( 75 * (VREFP - VREFN) * ADCVALUE) / 4096)
        ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;

        // Convert C to F  => F = ( ( C * 9 ) + 160 ) / 5
        ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
    }
}
