/**************************************************************************//**
 * @file        fileManagement.c
 * @brief       File managemet
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2022.07.06
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/
#include "fileManagement.h"

#include <stdio.h>     // Standard input/output definitions
#include <stdlib.h>
#include <string.h>    // String function definitions

/*******************************************************************************
******************************************************************************/
int createFile(char *str)
{
   struct timeval tv;
   FILE * pFile;

   gettimeofday(&tv,NULL);
   //printf("%X  %X\n", tv.tv_sec - TIMESTAMP, tv.tv_usec);

   //sprintf(str,"./log_MB7092/ToFraw.csv");
   sprintf(str,"./log_TDC7200/ToFraw.csv");
   //sprintf(str,"./log_RD3000/Ultrasound_%ld.txt", tv.tv_sec);
   //printf(" file:  %s \n", str);
   pFile = fopen(str,"r");
   //printf(" pFile: %s \n",pFile);

   return 0;
}

