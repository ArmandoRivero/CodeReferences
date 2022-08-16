/**************************************************************************//**
 * @file        TDC7200_ToF.h
 * @brief       Time of Flight calculation
 * @author      Armando Rivero
 * @date        c: 2019.06.03
 * @date        m: 2019.06.03
 * @version     0.01
 *****************************************************************************/
#ifndef __TDC7200TOF_H
#define __TDC7200TOF_H


#define TDC7200_CLOCKFREQ 8000000

#define STOP_SAMPLES_x1 0
#define STOP_SAMPLES_x2 0x01
#define STOP_SAMPLES_x3 0x02
#define STOP_SAMPLES_x4 0x03
#define STOP_SAMPLES_x5 0x04

#define CALIB_PER_x2   2
#define CALIB_PER_x10  10
#define CALIB_PER_x20  20
#define CALIB_PER_x40  40

/**
 * @brief   Convert Hex to Decimal
 * @param   *str String to be converted to Dec format
 */
float TDC7200_ToF(float cal1,float cal2,float time1,float time2,float clkCnt1);


#endif
