//
// Created by vedan on 4/28/2024.
//

#include "expression.h"
#include <string>
#include "Arduino.h"

String analyzeBytes(uint32_t buttonBits) {
    String result = ""; // Initialize an empty result string

    // Interpret the button bits using a switch statement
    switch (buttonBits) {
        case 0b0111111101111111: // Enter
            // Implement your logic for Enter button
            result = "Enter";
            break;
        case 0b1011111110111111: // +
            // Implement your logic for + button
            result = "+";
            break;
        case 0b110111111111111111111111: // -
            // Implement your logic for - button
            result = "-";
            break;
            // Add cases for other buttons as needed
            // ...

        default:
            // Handle unrecognized button combinations
            result = "Unknown";
            break;
    }

    return result;
}