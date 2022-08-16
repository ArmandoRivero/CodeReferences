/**************************************************************************//**
 * @file        hex2dec.h
 * @brief       Conversion from ASCII-HEX to decimal
 * @author      Armando Rivero
 * @date        c: 2019.06.03
 * @date        m: 2019.06.03
 * @version     0.01
 *****************************************************************************/
#ifndef __HEX2DEC_H
#define __HEX2DEC_H

#define TIMEFIX   0x51280680 //Sat Feb 23 00:00:00 UTC 2013
#define MAX_TRY   5
#define LAST_FILE 24


/**
 * @brief   Convert Hex to Decimal
 * @param   *str String to be converted to Dec format
 */
unsigned int parseHex(const char *str);


#endif
