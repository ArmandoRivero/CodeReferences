/**************************************************************************//**
 * @file        TDC7200_ToF.c
 * @brief       Time of Flight calculation
 * @author      Armando Rivero
 * @date        c: 2019.06.03
 * @date        m: 2019.06.03
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/
#include "TDC7200_ToF.h"


/*******************************************************************************
******************************************************************************/
float TDC7200_ToF(float calibration1, float calibration2, float time1_reg, 
                  float time2_reg,float clock_count1_reg)
{
    float CLOCKperiod = 1.0/TDC7200_CLOCKFREQ;
    float calCount = 0;
    float normLSB = 0;
    float ToF_1 = 0;

    // Calculate the value of normLSB
    calCount = (calibration2-calibration1)/(CALIB_PER_x10-1);
    normLSB = CLOCKperiod/calCount;

    // Calculate ToF_1
    ToF_1 = normLSB*(time1_reg-time2_reg) + (clock_count1_reg*CLOCKperiod);
    return ToF_1;
}
