/*Display library

License Here
Company Here
*/

#include "Arduino.h"

#ifndef Display_H
#define Display_H

// Define types of displays.
#define commonAnode 1
#define commonCathode 2 

// Uncomment to enable printing out nice debug messages.
// #define SD_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef SD_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

#define SEND(...) { if (enable) { send(String(__VA_ARGS__)); }; }

class Display {
  public:
    Display(int *controlPins, int *dataPins, uint8_t type, int digits);
    
    void begin(void);

    void write(int data);

    void write(double data);

    void write(float data);

    void write(String data);

    void brightness(int percentage);

    bool enable;

    bool scroll;

    int scrollInterval;

    int dutyHigh;

    int dutyLow;

  private:
    int *_controlPins, *_dataPins, _type, _digits, _curr_digit;
    char _displayTable[50][2] = {{'0', 0x3F}, 
                                {'1', 0x06},
                                {'2', 0x5B},
                                {'3', 0x4F},
                                {'4', 0x66},
                                {'5', 0x6D},
                                {'6', 0x7D},
                                {'7', 0x07},
                                {'8', 0x7F},
                                {'9', 0x6F},
                                {' ', 0x00},
                                {'\'', 0x20},
                                {'-', 0x40},
                                {'.', 0x80},
                                {'=', 0x48},
                                {'[', 0x39},
                                {']', 0x0F},
                                {'_', 0x08},
                                {'*', 0x63},
                                {'a', 0x5F},
                                {'b', 0x7C},
                                {'c', 0x58},
                                {'d', 0x5E},
                                {'e', 0x7B},
                                {'f', 0x71},
                                {'g', 0x6F},
                                {'h', 0x74},
                                {'i', 0x10},
                                {'j', 0x0E},
                                {'l', 0x30},
                                {'n', 0x54},
                                {'o', 0x5C},
                                {'p', 0x73},
                                {'r', 0x50},
                                {'s', 0x6D},
                                {'t', 0x78},
                                {'u', 0x1C},
                                {'A', 0x77},
                                {'C', 0x39},
                                {'E', 0x79},
                                {'F', 0x71},
                                {'G', 0x7D},
                                {'H', 0x76},
                                {'I', 0x30},
                                {'J', 0x0E},
                                {'L', 0x38},
                                {'O', 0x3F},
                                {'P', 0x73},
                                {'S', 0x6D},
                                {'U', 0x3E}};

    void send(String data);
    bool findChar(char c);
    char getCharHash(char c);
};

#endif