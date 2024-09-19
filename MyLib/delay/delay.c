/*
 * delay.c
 *
 *  Created on: Sep 14, 2024
 *      Author: ahmad
 */


#include "delay.h"
//================================ Implementaion ======================
void DelayMs(uint32_t ui32Ms){
    /* First:
     * SysCtlClockGet() => clock rate (Hz)
     * Since the clock time (s) = 1 / clock rate (Hz)
     * then the clock time (s) = 1 / SysCtlClockGet()
     * so the clock time (ms) = 1 * 1000 / SysCtlClockGet() --> (1)
     * Second:
     * The loop of SysCtlDelay() = 3 clock cycles
     * then the 1 loop time = 3 clock time --> (2)
     *          1 loop time (ms) = 3 * (1 * 1000 /SysCtlClockGet())
     *          1 loop time (ms) = 3000 / SysCtlClockGet()
     *          2 loop time (ms) = 2 * 3000 / SysCtlClockGet()
     *          count            = ui32Ms --> (4)
     *          count = ui32Ms / [3000 / SysCtlClockGet()]
     *          count = ui32Ms * SysCtlClockGet() / 3000
     */

    SysCtlDelay(SysCtlClockGet() * ((float)ui32Ms / 3000.0));
}
void DelayUs(uint32_t ui32Ms){
    /* First:
     * SysCtlClockGet() => clock rate (Hz)
     * Since the clock time (s) = 1 / clock rate (Hz)
     * then the clock time (s) = 1 / SysCtlClockGet()
     * so the clock time (ms) = 1 * 1,000,000 / SysCtlClockGet() --> (1)
     * Second:
     * The loop of SysCtlDelay() = 3 clock cycles
     * then the 1 loop time = 3 clock time --> (2)
     *          1 loop time (ms) = 3 * (1 * 1,000,000 /SysCtlClockGet())
     *          1 loop time (ms) = 3000,000 / SysCtlClockGet()
     *          2 loop time (ms) = 2 * 3000,000 / SysCtlClockGet()
     *          count            = ui32Ms --> (4)
     *          count = ui32Ms / [3000 / SysCtlClockGet()]
     *          count = ui32Ms * SysCtlClockGet() / 3000,000
     */

    SysCtlDelay(SysCtlClockGet() * ((float)ui32Ms / 3000000.0));
}


