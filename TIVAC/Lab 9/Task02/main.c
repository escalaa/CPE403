#include <stdint.h>
#include <stdbool.h>
#include <math.h>           //  enables us to use the sinf()
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"  // enables us to use Floating Point Units
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
//
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define SERIES_LENGTH 100   // the depth of our data
float gSeriesData[SERIES_LENGTH];   // an array of floats
int32_t i32DataCount = 0;       // counter for math loop
int main(void)
{
    float fRadians; // calculate sine
    float sine;
    float cosine;
    ROM_FPULazyStackingEnable();
    ROM_FPUEnable(); // Enable Floating Points
    // 50MHz System Clock
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    fRadians = ((2 * M_PI) / SERIES_LENGTH);  // getting a full sine wave
    // Calculate the sine value for each angle
    while(i32DataCount < SERIES_LENGTH)
    {
        sine = sinf(fRadians * 50 * i32DataCount);
        cosine = cosf(fRadians * 200 * i32DataCount);
        gSeriesData[i32DataCount] = 1.5 + sine + cosine;
        i32DataCount++;
    }
    while(1)
    {
    }

}
