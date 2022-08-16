#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Author: Armando Rivero
# Date:   2019.07.24
# Modif:
# Ver.:   1.0
# =============================================================================

import socket


# HOST = '192.168.1.154'#Enter IP or Hostname of your serveri
HOST = '10.11.15.154'#Enter IP or Hostname of your serveri
PORT = 8000 # Pick an open Port (1000+ recomended) , must match the serveur port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#Lets loop awaiting for your input
while True:
    command = input('Enter command: ')


    if command == 'exit':
    	s.close()
    	print('socket closed!')

    s.send(command.encode("utf-8"))
    reply = s.recv(1024)
    if reply.decode('utf-8')=='Terminate':
        break



    print (reply.decode("utf-8"))

print ('Closing communication')

s.close()

# initImgAcqu.1.MM_G/2019-07-25-07-32-03/
