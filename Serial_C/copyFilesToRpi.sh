#!/bin/bash
# author: Armando Rivero
# date:   2019.05.21
# modif.: 2019.05.21
# ver.:   1.0


# connect to Raspberry pi using sshpass, verify that the SW is installed
password="raspberry"
username="pi"
IPaddress="10.11.15.66"
pathToCopy="/home/pi/Documents/DevelRD3k/"
pathSource="../Serial_C/"


echo "--------------------------"
echo "executing script on Raspbery Pi...."

sshpass -p "$password" ssh -l $username $IPaddress "cd ${pathToCopy}; pwd; ls;" 

# copy files from local to Rpi
echo "copying files to Rapberry Pi"
sshpass -p "$password" scp -r $pathSource $username@$IPaddress:$pathToCopy 
echo "files copied into RPi"
echo "--------------------------"
