#include <sstream>
#include "Arduino.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "expression.h"
#include "calc.h"

const int dataPin = 17;   /* Q7 */
const int clockPin = 21;  /* CP */
const int latchPin = 22;  /* PL */
const int numBits = 24;   /* Set to 8 * number of shift registers */
String expression = "";

boolean display1 = true;
boolean display2 = true;
boolean display3 = true;
boolean display4 = true;
boolean display5 = true;
boolean display6 = true;
boolean display7 = true;
boolean display8 = true;
boolean display9 = true;
double ox , oy ;

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void Graph(TFT_eSPI &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw) {

    double ydiv, xdiv;
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    double i;
    double temp;
    int rot, newrot;

    if (redraw == true) {

        redraw = false;
        ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
        oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
        // draw y scale
        for ( i = ylo; i <= yhi; i += yinc) {
            // compute the transform
            temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

            if (i == 0) {
                d.drawLine(gx, temp, gx + w, temp, acolor);
            }
            else {
                d.drawLine(gx, temp, gx + w, temp, gcolor);
            }

            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(gx - 40, temp);
            // precision is default Arduino--this could really use some format control
            d.println(i);
        }
        // draw x scale
        for (i = xlo; i <= xhi; i += xinc) {

            // compute the transform

            temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
            if (i == 0) {
                d.drawLine(temp, gy, temp, gy - h, acolor);
            }
            else {
                d.drawLine(temp, gy, temp, gy - h, gcolor);
            }

            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(temp, gy + 10);
            // precision is default Arduino--this could really use some format control
            d.println(i);
        }

        //now draw the labels
        d.setTextSize(2);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(gx , gy - h - 30);
        d.println(title);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx , gy + 20);
        d.println(xlabel);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx - 30, gy - h - 10);
        d.println(ylabel);


    }

    //graph drawn now plot the data
    // the entire plotting code are these few lines...
    // recall that ox and oy are initialized as static above
    x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
    y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d.drawLine(ox, oy, x, y, pcolor);
    d.drawLine(ox, oy + 1, x, y + 1, pcolor);
    d.drawLine(ox, oy - 1, x, y - 1, pcolor);
    ox = x;
    oy = y;

}

static void print_help() {
    tft.setTextSize(2);
    tft.println("Usage: calc [expression [expression] ...]");
    tft.println("Functions and Operators:");
    tft.println("Basic arithmethic:  +, -, *, /, ^, mod");
    tft.println("Trigonometrics:     sin(x), cos(x), tan(x), asin(x), acos(x), atan(x), cot(a)");
    tft.println("Hyperbolics:        sinh(x), cosh(x), tanh(x), asinh(x), acosh(x), atanh(x)");
    tft.println("Exponentials:       sqr(x), sqrt(x), cbrt(x), root(n, x), exp(x)");
    tft.println("Logarithms:         log2(x), log10(x), ln(x), log(b, e)");
    tft.println("Miscelleneous:      min(a, b), max(a, b), sign(x), hypot(x, y)");
    tft.println("Rounding functions: ceil(x), floor(x), abs(x), trunc(x)");
    tft.println("Combinatorics:      n!, binomial(n, k)");
    tft.println("Circle and Ball:    circumference(r), circle_area(r), ball_volume(r), ball_surface(r)");
    tft.println("Constants:          e, pi, inf");
}

static std::string strip_trailing_zeros(const std::string &result) {
    std::string str;
    if (result.find('.') != std::string::npos) {
        int i = (int) result.size() - 1;
        while (i >= 0 and result[i] == '0')
            --i;
        str = result[i] == '.' ? result.substr(0, (unsigned long) i) : result.substr(0, (unsigned long) i + 1);
    } else {
        str = result;
    }

    return str != "-0" ? str : "0";
}

static std::string to_string(double r) {
    std::stringstream oss;
    std::string str;
    return strip_trailing_zeros(str);
}

void setup() {
    Serial.begin(9600);
    pinMode(dataPin, INPUT_PULLDOWN);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    tft.init();
    tft.setRotation(1);
}

// Function to print a 24-bit number in binary with leading zeros
void printBits(uint32_t value, byte numBits) {
    for (byte i = numBits - 1; i < 255; i--) {
        tft.print(bitRead(value, i));
    }
}

void loop() {
    // Step 1: Sample
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);

    // Step 2: Shift
    uint32_t buttonBits = 0;
    for (int i = 0; i < numBits; i++) {
        int bit = digitalRead(dataPin);
        buttonBits |= ((uint32_t)bit << i);
        digitalWrite(clockPin, HIGH); // Shift out the next bit
        digitalWrite(clockPin, LOW);
    }

    String result = analyzeBytes(buttonBits);
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0,2);
    tft.setTextSize(10);
    char buf[50];
    std::string expression1 = "pi*8";
    sprintf(buf, "%.9f", calc::evaluate(expression1)); // This will format the float to 9 decimal places
    tft.print(buf);
    delay(5000);

    // Print the button bits on the TFT display
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.print("Button Bits: ");
    printBits(buttonBits, numBits); // Use the new function to print the bits
    tft.println();

    Serial.println(result);

    // Interpret the button bits
    // TODO: Update your switch cases to handle 24-bit values
    // ...

    tft.print(expression);

    delay(100);
}
