#!/bin/bash
# author: Armando Rivero
# date:   2019.05.21
# modif.: 2019.05.31
# ver.:   1.0


fileCompiledName="run"
folderPath="./SerialLogger/"
file1="mainSerialLogger_xxx.c"
file2="serialConfig.c"
file3="fileManagement.c"

echo "--------------------------"
if [ "$#" -eq  "0" ]
   then
     echo "No argument supplied"
     echo "Please insert path and file name"
 else
	echo "compiling...."
	gcc -Wall $1 $folderPath$file2 $folderPath$file3 -o $fileCompiledName
	echo "complied"
 fi

 	
echo "--------------------------"
