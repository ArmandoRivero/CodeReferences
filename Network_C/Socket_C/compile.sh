#!/bin/bash
# author: Armando Rivero
# date:   2020.03.06
# modif.: 2021.04.13
# ver.:   1.03

echo "erasing run_Slave"
rm run_Slave

echo "compiling slaveRpi.c"
gcc networkSlave.c -o run_Slave

