/*
 * debug.h
 *
 *  Created on: Aug 26, 2024
 *      Author: joshuamilnes
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef void (*debugCallback)(int);  // Function pointer type

void debug_init(UART_HandleTypeDef* uart);
void debug_msg(char* str);
void debug_registerCallback(debugCallback f);
void debug_main(void);

#ifdef __cplusplus
}
#endif

#endif // DEBUG_H
