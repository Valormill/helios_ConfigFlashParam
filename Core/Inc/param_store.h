/*
 * param_store.cpp
 *
 *  Created on: September 12, 2024
 *      Author: Blaze Moore
 *
 */

#ifndef INC_PARAM_STORE_H_
#define INC_PARAM_STORE_H_

#include <string>
#include <stdint.h>

//-----------------------------------------------------------------------------
//
// Public Definitions
//
//-----------------------------------------------------------------------------
#define INT_ADDR 0
#define INT_LEN  2
#define INT_SIZE 4

#define STR_ADDR (INT_ADDR + INT_SIZE * INT_LEN)
#define STR_SIZE 20
#define STR_LEN  2

#define PARAMS_STORE_KEYS_DEFAULT_REV 0
#define PARAMS_STORE_KEYS_PNUM 8

extern const UINT32 params_defaults[PARAMS_STORE_KEYS_PNUM];
extern const std::string params_defaults_str[STR_LEN];


//-----------------------------------------------------------------------------
//
// Public Functions
//
//-----------------------------------------------------------------------------

// Initialize parameter store, loading from flash or setting defaults
void param_store_init();

// Get all parameters from flash and print them
void param_store_getAll();

// Build a map of parameters for fast access
void param_store_buildMap();

// Set an integer parameter and write it to flash
void param_store_setIntItem(int key, int var);

// Get an integer parameter, reading from flash
int param_store_getIntItem(int key);

// Set a string parameter and write it to flash
void param_store_setString(int key, const char* str);

// Get a string parameter, reading from flash
const char* param_store_getStringItem(int key);

// Check if flash data matches default values, restore if needed
char param_checkStoreDefaults();

// Dump all parameters into flash using the config API
void param_store_dump();

#endif /* INC_PARAM_STORE_H_ */

