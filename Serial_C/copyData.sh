#!/bin/bash
# author: Armando Rivero
# date:   2019.06.03
# modif.: 2019.06.03
# ver.:   1.0


pathDestination="./TDC7200_ToF/DataFolder"
pathSource="./SerialLogger/log_TDC7200/*.*"


echo "--------------------------"
echo "copying files from SerialLogger to TDC7200_ToF "
cp $pathSource $pathDestination
echo "--------------------------"
