/**************************************************************************//**
 * @file        fileManagement.h
 * @brief       File managemet
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2019.05.31
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/
#ifndef __FILEMANAGEMENT_H
#define __FILEMANAGEMENT_H

/**
 * @brief   create a file with the timestamp
 * @param   *str = 8-Byte hex timestamp + ".txt" 
 */
int createFile(char *str);


#endif
