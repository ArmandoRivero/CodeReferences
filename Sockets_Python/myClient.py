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
print ('Client: Socket created')

HOST = '' # Server IP or Hostname
PORT = 8000 # Pick an open Port (1000+ recommended), must match the client's port
s.connect((HOST, PORT))

msg_buff = ''

while True:
	msg = s.recv(7)

	if len(msg) <= 0:
		break

	msg_buff += msg.decode("utf-8")
	print('\tmsg:      '+msg.decode("utf-8"))
	print('\tmsg_buff: '+msg_buff)

print(f"message received: {msg_buff}")
