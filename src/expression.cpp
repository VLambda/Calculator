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
        case 0b011111111111111111111111:
            result = "Menu";
            break;
        case 0b101111111111111111111111:
            result = "Left";
            break;
        case 0b110111111111111111111111:
            result = "+"
            break;
        case 0b111011111111111111111111:
            result = "-";
            break;
        case 0b111101111111111111111111:
            result = "7";
            break;
        case 0b111110111111111111111111:
            result = "4";
            break;
        case 0b111111011111111111111111:
            result = "1";
            break;
        case 0b111111101111111111111111:
            result = "0";
            break;
        case 0b111111110111111111111111:
            result = "Up";
            break;
        case 0b111111111011111111111111:
            result = "Down";
            break;
        case 0b111111111101111111111111:
            result = "*";
            break;
        case 0b111111111110111111111111:
            result = "/";
            break;
        case 0b111111111111011111111111:
            result = "8";
            break;
        case 0b111111111111101111111111:
            result = "5";
            break;
        case 0b111111111111110111111111:
            result = "2";
            break;
        case 0b111111111111111011111111:
            result = ".";
            break;
        case 0b111111111111111101111111:
            result = "Shift";
            break;
        case 0b111111111111111110111111:
            result = "Right";
            break;
        case 0b111111111111111111011111:
            result = "Power";
            break;
        case 0b111111111111111111101111:
            result = "x";
            break;
        case 0b111111111111111111110111:
            result = "9";
            break;
        case 0b111111111111111111111011:
            result = "6";
            break;
        case 0b111111111111111111111101:
            result = "3";
            break;
        case 0b111111111111111111111110:
            result = "Enter";
            break;
        default:
            // Handle unrecognized button combinations
            break;
    }

    return result;
}
