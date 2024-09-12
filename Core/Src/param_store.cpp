/*
 * param_store.cpp
 *
 *  Created on: September 12, 2024
 *      Author: Blaze Moore
 */

#include <map>
#include <string>
#include <cstring>  // For strcmp
#include "flash_program.h"
#include "defs.h"
#include "param_store.h"
#include "config.h"  // Include config for persistent storage
#include <vector>
#include "util.h"
#include "debug.h"
#include <stdio.h>  // For sprintf

//-----------------------------------------------------------------------------
//
// Local Definitions
//
//-----------------------------------------------------------------------------

#include <string>

const UINT32 params_defaults[PARAMS_STORE_KEYS_PNUM] = {
    0xDADEC040, 0, 0, 0, 0, 0, 0, 0
    // Add other default values as needed
};

const std::string params_defaults_str[STR_LEN] = {
    "default_string_1", "default_string_2"
    // Add other default string values
};

//-----------------------------------------------------------------------------
//
// Local Variables
//
//-----------------------------------------------------------------------------

int intPair[INT_LEN];
const char* strPair[STR_LEN];
int params[INT_LEN];

//-----------------------------------------------------------------------------
//
// Local Functions
//
//-----------------------------------------------------------------------------

void param_store_init()
{
    // Load all parameters from flash or set defaults
    for (int i = 0; i < INT_LEN; i++) {
        intPair[i] = configGetInt(i);  // Load from config
        if (intPair[i] == static_cast<int>(-1)) {  // If not found, set default
            intPair[i] = params_defaults[i];
            configWrite("defaultInt", i, 'i', &params_defaults[i]);  // Write default to flash
        }
    }

    for (int i = 0; i < STR_LEN; i++) {
        const char* val = configGetString(i);
        if (val == nullptr || val[0] == '\0') {  // If not found, set default
            strPair[i] = params_defaults_str[i].c_str();
            configWrite("defaultStr", i, 's', params_defaults_str[i].c_str());
        } else {
            strPair[i] = val;
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void param_store_getAll()
{
    // Load configuration data from flash
    const char* result = loadConfig();

    // Check if the configuration was successfully loaded
    if (strcmp(result, "Success: Config loaded from flash.") == 0) {
        // Configuration data successfully loaded; proceed with processing
        for (int i = 0; i < INT_LEN + STR_LEN; i++) {
            char num_string[20];
            sprintf(num_string, "%u", params[i]);  // Use sprintf for formatting
//            debug_msg(num_string);
        }
    } else {
        // If an error occurred during loading, print the error message
//        debug_msg(result);
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void param_store_buildMap()
{
    // Build a map of parameter values for easy access
    for (int i = INT_ADDR; i < INT_ADDR + INT_LEN; i++) {
        intPair[i] = configGetInt(i);  // Fetch from config
    }

    for (int i = STR_ADDR; i < STR_ADDR + STR_LEN; i++) {
        strPair[i] = configGetString(i);  // Fetch from config
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void param_store_setIntItem(int key, int var)
{
    // Set an integer parameter and persist it to flash using config
    params[key] = var;
    configWrite("paramInt", key, 'i', &var);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int param_store_getIntItem(int key)
{
    // Get an integer parameter, using config as the backing store
    return configGetInt(key);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

const char* param_store_getStringItem(int key)
{
    // Get a string parameter, using config as the backing store
    return configGetString(key);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void param_store_setString(int key, const char* str)
{
    // Set a string parameter and persist it to flash using config
    strPair[key] = str;
    configWrite("paramStr", key, 's', str);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

char param_checkStoreDefaults()
{
    // First, check the revision in the config
    int currentRev = configGetInt(PARAMS_STORE_KEYS_DEFAULT_REV);

    // If the revision isn't found or doesn't match the default, restore defaults
    if (currentRev == -1 || currentRev != params_defaults[PARAMS_STORE_KEYS_DEFAULT_REV]) {
//        debug_msg((char*) "Restoring default settings");

        // Reinitialize and write all default parameters
        param_store_init();  // Reset all parameters to defaults

        // Write the default revision to the config
        int defaultRev = params_defaults[PARAMS_STORE_KEYS_DEFAULT_REV];
        configWrite("defaultRev", PARAMS_STORE_KEYS_DEFAULT_REV, 'i', &defaultRev);

        // Write default integer parameters to the config
        for (int i = 0; i < INT_LEN; i++) {
            int defaultIntValue = params_defaults[i];
            configWrite("defaultInt", i, 'i', &defaultIntValue);
        }

        // Write default string parameters to the config
        for (int i = 0; i < STR_LEN; i++) {
            const char* defaultStrValue = params_defaults_str[i].c_str();
            configWrite("defaultStr", i, 's', defaultStrValue);
        }

        return 1;  // Indicate that defaults were restored
    }

    // Otherwise, check each individual integer and string parameter
    for (int i = 0; i < INT_LEN; i++) {
        int storedIntValue = configGetInt(i);
        if (storedIntValue == -1 || storedIntValue != params_defaults[i]) {
            // Restore the missing or incorrect integer value
            configWrite("defaultInt", i, 'i', &params_defaults[i]);
        }
    }

    for (int i = 0; i < STR_LEN; i++) {
        std::string storedStrValue = configGetString(i);
        if (storedStrValue.empty() || storedStrValue != params_defaults_str[i]) {
            // Restore the missing or incorrect string value
            configWrite("defaultStr", i, 's', params_defaults_str[i].c_str());
        }
    }

    return 0;  // Indicate that no defaults were restored
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void param_store_dump()
{
    // Flush the config to save all parameters into flash
    const char* result = flashConfig();

    // Check if the flash operation was successful
    if (strcmp(result, "Success: Config data written to flash.") == 0) {
//        debug_msg((char*)"Success: Parameters dumped to flash.");
    } else {
//        debug_msg(result);  // Log the error message returned by flashConfig
    }
}

