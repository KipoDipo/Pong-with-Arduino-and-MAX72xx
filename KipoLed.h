#pragma once
#include <Arduino.h>
#include <LedControl.h>

class Display
{
public:
    Display(int DIN, int CS, int CLK, int count, int intensity = 3)
        : display(DIN, CLK, CS, count)
    {
        for (int i = 0; i < count; i++)
        {
            display.shutdown(i, false);
            display.setIntensity(i, intensity);
            display.clearDisplay(i);
        }
    }
    
    void setLed(int x, int y, bool state)
    {
        display.setLed(x / 8, x - ((x / 8) * 8), 8 - y - 1, state);
    }

    void setRow(int row, byte data)
    {
        for (int i = 0; i < 8; i++)
            setLed(8 - 1 - i, row, data & (1 << i));
    }

    void setRow(int row, unsigned data)
    {
        for (byte i = 0; i < 16; i++)
            setLed(i, row, data & (1 << (15 - i)));	
    }

    void setAll(byte data[8])
    {
        for (size_t j = 0; j < 8; j++)
        {
            byte a = 0;
            for (size_t i = 0; i < 8; i++)
            {
                a |= (1 & (data[i] >> (8 - 1 - j))) << i;
            }
            display.setRow(0, j, a);
        }

    }

    void setAll(unsigned data[8])
    {
        for (size_t j = 0; j < 8; j++)
        {
            byte a = 0, b = 0;
            for (size_t i = 0; i < 8; i++)
            {
                a |= (1 & (data[i] >> (16 - 1 - j))) << i;
                b |= (1 & (data[i] >> (8  - 1 - j))) << i;
            }
            display.setRow(0, j, a);
            display.setRow(1, j, b);
        }
    }

private:
    LedControl display;
};