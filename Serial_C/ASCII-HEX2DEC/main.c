/**************************************************************************//**
 * @file        main.c
 * @brief       Conversion from ASCII-HEX to decimal
 * @author      Armando Rivero
 * @date        c: 2013.03.04
 * @date        m: 2019.06.03
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/
/*****************************************************************************
 * NOTES:
 *       This project convert the ASCII-HEX files stored in 
 *       a file to a DEC format
 *       
 *****************************************************************************/

#include <stdio.h>     // Standard input/output definitions
#include <stdlib.h>
#include <string.h>    // String function definitions
#include <unistd.h>    // UNIX standard function definitions
#include <fcntl.h>     // File control definitions
#include <errno.h>     // Error number definitions
#include <termios.h>   // POSIX terminal control definitions
#include <sys/time.h>  // System Time control definitions
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


#include "ascii-hex2dec.h"


/*******************************************************************************
******************************************************************************/
int main(void)
{
   char sourceFile[32];
   char nameFileDec[32];
   //char sourceFileDec[32];
   char *pDec;
   char *folderPath = "./DataFolder/\0";
   char *fileName = "dataFile.csv\0";
   char *sourceFileDec = "dataFile_dec.csv\0";

   int conversionCounter = 0;
   
   unsigned int dec1,dec2,dec3,dec4,dec5;
   
   char decString[1024] = {0};
   
   //token pointers
   const char *date;
   const char *hour;
   const char *data1;
   const char *data2;
   const char *data3;
   const char *data4;
   const char *data5;
   
   FILE *pFile;
   FILE *pFileDec;
   
   // create Log folder
   struct stat st = {0};
   if(stat("./DataFolder",&st)==-1)
      mkdir("./DataFolder",0777);
   
   //struct termios tio;

   printf("\n***********************************************************\n");
   printf("*     ------ Starting ASCII-HEX to DEC conversion ------\n");
   printf("*                                      by: A. Rivero\n");
   printf("***********************************************************\n");
   

   //sprintf(nameFileDec, "%s",fileName);
   //sprintf(sourceFile, "%s%s", folderPath ,nameFileDec);
   sprintf(sourceFile, "%s%s", folderPath ,fileName);
   sprintf(nameFileDec, "%s%s", folderPath ,sourceFileDec);
   
   /** Opening log file */
   pFile = fopen(sourceFile,"r");
   if(pFile == NULL)
   {
      printf("Problem to open file : %s\n", sourceFile);
   }
   printf("\n--> File '%s' succesfully open!\n", sourceFile);
   
   /** Creating log file for data in Decimal format*/
   //decFile(sourceFileDec);
   pFileDec = fopen(nameFileDec,"w");
   if(pFileDec == NULL)
   {
         printf("Problem to open Decfile: %s\n", sourceFileDec);
         //~ continue;//exit(1);
         exit(1);
   }
   printf("\n--> DecFile '%s' succesfully created!\n", sourceFileDec);
   
   
   /** Reading line by line and writing data in log file */
   if ( pFile != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      
      printf("\n--> Starting conversion from ASCII-HEX to DEC\n");
      while ( fgets ( line, sizeof line, pFile ) != NULL ) /* read a line */
      {
         pDec = line;
         printf("%d --------------------------------\n",conversionCounter);
         printf("HEX-> %s",line);
         //~ fputs ( line, stdout ); /* write the line */
         
         date     = strtok(pDec, ","); //data
         hour     = strtok(NULL, ","); //node identifier
         data1    = strtok(NULL, ","); //RSSI
         data2    = strtok(NULL, ","); //LQI
         data3    = strtok(NULL, ","); //Timestamp
         data4    = strtok(NULL, ","); //Timestamp
         data5    = strtok(NULL, "\n\r"); //Timestamp Linux
         
         
         dec1 = parseHex(data1);
         dec2 = parseHex(data2);
         dec3 = parseHex(data3);
         dec4 = parseHex(data4);
         dec5 = parseHex(data5);
         
         sprintf(decString, "%s,%s,%u,%u,%u,%u,%u\n\r", 
                             date,hour,dec1,dec2,dec3,dec4,dec5);
         printf("DEC-> %s",decString);
         
         /** Writing data in log file */
         fwrite(decString, 1, strlen(decString)-1, pFileDec);

         conversionCounter++;
      }
      printf("\n--> Conversion finished!\n");
      printf("***********************************************************\n\n");
   }
   else
   {
      perror ( sourceFile ); /* why didn't the file open? */
   }
   
   fclose(pFileDec);
   fclose(pFile);
   
   return 0;
}
