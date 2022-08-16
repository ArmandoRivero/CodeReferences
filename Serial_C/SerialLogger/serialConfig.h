/**************************************************************************//**
 * @file        serialConfig.h
 * @brief       Serial Logger configuration
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2019.05.23
 * @version     0.01
 *****************************************************************************/
#ifndef __SERIALCONFIG_H
#define __SERIALCONFIG_H

#define MAX_TRY   5

/* Serial port configuration */
#define BAUDRATE   B9600
#define DATABITS   CS8
#define STOPBITS   1
#define PARITYON   0
#define PARITY	   0


/* Timestamp configuration  */
#define TIMEFIX  0


/**
 * @brief   open target serial port
 * @param   *serialName Name of the port to open
 * @return  -1 if error
 */
int open_port(char *serialName);

/**
 * @brief   configure serial port
 * @param   BAUDRATE
 * @return  -1 if error
 */
int configure_serial(char *baudrate);


/**
 * @brief   write data to a port
 * @param   fd File descriptor
 * @param   *buf Buffer of data to write
 * @param   size Number of bytes to write
 */
int write_port(int fd, const char *buf, int size);


/**
 * @brief   read serial port until \n\n\r is found
 * @param   file descriptor
 * @param   *buf Buffer of data read
 * @param   size Number of bytes to read
 * @return  count Number of bytes read, -1 if error
 */
int read_port(int fd, char *buf, int size);

/**
 * @brief   close open port
 * @param   fd File descriptor
 */
int close_port(int fd);


/**
 * @brief   get time of the day realtive to TIMESTAMP value
 * @param   *str = 8-Byte hex timestamp
 */
int time2string(char *str);

#endif

