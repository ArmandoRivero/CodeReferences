#!/bin/bash
# author: Armando Rivero
# date:   2020.03.06
# modif.: 2020.03.06
# ver.:   1.02

echo "erasing test_slaveRpi"
rm test_slaveRpi
rm run_single

echo "compiling slaveRpi.c"
gcc slaveRPi.c -o test_slaveRpi
gcc slaveRPi_singleSocket.c -o run_single

