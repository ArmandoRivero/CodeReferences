#!/bin/bash
# author: Armando Rivero
# date:   2019.05.21
# modif.: 2019.05.21
# ver.:   1.0


fileCompiledName="run"
pathOfFile="./C_code/serialLogger.c"

echo "--------------------------"
if [ "$#" -eq  "0" ]
   then
     echo "No arguments supplied"
 else
 	echo "compiling...."
	gcc $1 -o $fileCompiledName
	echo "complied"
 fi
echo "--------------------------"
