#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Author: Armando Rivero
# Date:   2019.07.26
# Modif:
# Ver.:   1.0
# =============================================================================
# based on: https://www.youtube.com/watch?v=T0rYSFPAR0A
# =============================================================================

import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ('Server: Socket created')

HOST = '' # Server IP or Hostname
PORT = 8000 # Pick an open Port (1000+ recommended), must match the client's port


#managing error exeption
try:
    s.bind((HOST, PORT))
except socket.error:
    print ('Bind failed')

s.listen(1)
print ('Socket awaiting messages')

while True:
    (clt, addr) = s.accept()
    print("connection to: {}".format(addr))
    msgTosend = "Socket programing in python3"
    clt.send(msgTosend.encode("utf-8"))
    clt.close()
