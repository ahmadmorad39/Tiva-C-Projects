/*
 * delay.h
 *
 *  Created on: Sep 14, 2024
 *      Author: ahmad
 */

#ifndef DELAY_H_
#define DELAY_H_

/* Defines integer and boolean data types */
#include <stdint.h>
#include <stdbool.h>

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

//============================== Function prototype ================================
/**
 ***********************************************************************************
 * @brief provides a delay in milliseconds
 * @param The value of milliseconds
 *         The Maximum value at 80 MHz is 161061
 * @retrun None
 ************************************************************************************
 */

extern void DelayMs(uint32_t ui32Ms);
/**
***********************************************************************************
* @brief provides a delay in microseconds
* @param The value of milliseconds
*         The Maximum value at 80 MHz is 161,061,273
* @retrun None
************************************************************************************
*/

extern void DelayUs(uint32_t ui32Us);


#endif /* DELAY_H_ */
