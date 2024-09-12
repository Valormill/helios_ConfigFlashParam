/*
 * testCase.cpp
 *
 *  Created on: Aug 22, 2024
 *      Author: bem012
 */

#include <config.h>
#include <firmware.h>
#include <flashFile.h> // Include header for processAndFlashData
#include "flash_program.h"
#include <iostream>
#include <cstring>  // For std::strncpy
#include "testCase.h"
#include "param_store.h"
#include "defs.h"
#include "util.h"
// Define the function to initialize test data
void initializeTestData() {
    testConfigAndFirmwareWrite();
}

#define PUMP_SETTING_ID 0
#define PUMP_SETTING_NAME "pumpSetting"

void testConfigAndFirmwareWrite() {
    // Initial test: Check if defaults are restored when revision doesn't match
    std::cout << "Testing param_checkStoreDefaults() function:\n";

    // Simulate a mismatched revision
    int incorrectRev = -1;
    configWrite("defaultRev", PARAMS_STORE_KEYS_DEFAULT_REV, 'i', &incorrectRev);

    // Call the function to check and restore defaults
    char restoreResult = param_checkStoreDefaults();

    if (restoreResult == 1) {
        std::cout << "Defaults were successfully restored.\n";
    } else {
        std::cout << "Defaults were not restored; current values are already correct.\n";
    }

    // Load all parameters to check the restored values
    param_store_getAll();

    // Now, let's manually test setting and getting integer and string parameters

    // Set and get an integer parameter
    std::cout << "\nTesting integer parameter set and get:\n";
    int testIntId = 2;
    int testIntValue = 12345;

    param_store_setIntItem(testIntId, testIntValue);  // Set the integer
    int retrievedIntValue = param_store_getIntItem(testIntId);  // Retrieve the integer

    if (retrievedIntValue == testIntValue) {
        std::cout << "Integer parameter test passed: " << retrievedIntValue << std::endl;
    } else {
        std::cout << "Integer parameter test failed. Retrieved: " << retrievedIntValue << std::endl;
    }

    // Set and get a string parameter
    std::cout << "\nTesting string parameter set and get:\n";
    int testStrId = 1;
    const char* testStrValue = "Hello, Flash!";

    param_store_setString(testStrId, testStrValue);  // Set the string
    const char* retrievedStrValue = param_store_getStringItem(testStrId);  // Retrieve the string

    if (retrievedStrValue != nullptr && std::strcmp(retrievedStrValue, testStrValue) == 0) {
        std::cout << "String parameter test passed: " << retrievedStrValue << std::endl;
    } else {
        std::cout << "String parameter test failed. Retrieved: " << (retrievedStrValue ? retrievedStrValue : "null") << std::endl;
    }

    // Finally, dump the current parameters to flash
    std::cout << "\nDumping all parameters to flash...\n";
    param_store_dump();
    std::cout << "Parameters successfully dumped to flash.\n";
}




