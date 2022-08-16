/**************************************************************************//**
 * @file        hex2dec.c
 * @brief       Conversion from ASCII-HEX to decimal
 * @author      Armando Rivero
 * @date        c: 2013.03.04
 * @date        m: 2019.06.03
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/
#include "hex2dec.h"


/*******************************************************************************
******************************************************************************/
unsigned int parseHex(const char *str)
{
    unsigned int val = 0;
    char c;

    while((c = *str++))
    {
        val <<= 4;

        if (c >= '0' && c <= '9')
        {
            val += c & 0x0F;
            continue;
        }

        c &= 0xDF; // mask also range 'a' to 'f'
        if (c >= 'A' && c <= 'F')
        {
            val += (c & 0x07) + 9;
            continue;
        }

        return 0;
    }

    return val;
}

