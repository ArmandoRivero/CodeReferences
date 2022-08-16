#!/bin/bash
# author: Armando Rivero
# date:   2019.06.03
# modif.: 2019.06.03
# ver.:   1.0

echo "running Make on SerialLogger "
cd ./SerialLogger/ && make clean

cd ..

echo "running Make on TDC7200_ToF"
cd ./TDC7200_ToF/ && make clean
