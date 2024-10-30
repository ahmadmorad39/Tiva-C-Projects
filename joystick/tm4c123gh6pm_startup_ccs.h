/*
 * tm4c123gh6pm_startup_ccs.h
 *
 *  Created on: Oct 18, 2024
 *      Author: ahmad
 */

#ifndef TM4C123GH6PM_STARTUP_CCS_H_
#define TM4C123GH6PM_STARTUP_CCS_H_

/* IMPORTANT: Increase the heap size to support printf efficiently. */
void uart0_init(void);
void txchar(char c);

#endif /* TM4C123GH6PM_STARTUP_CCS_H_ */
