#!/bin/bash
# author: Armando Rivero
# date:   2020.03.09
# modif.: 2020.03.09
# ver.:   1.00

echo "erasing compiled files"
rm thread_network

echo "compiling..."
gcc thread_network.c -lpthread -o thread_network

echo "compilation finished"
