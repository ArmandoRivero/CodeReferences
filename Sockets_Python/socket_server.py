#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Author: Armando Rivero
# Date:   2019.07.24
# Modif:
# Ver.:   1.0
# =============================================================================

import socket


# HOST = '172.16.159.132' # Server IP or  Hostname
HOST = '' # Server IP or  Hostname
PORT = 8000 # Pick an open Port (1000+ recommended), must match the client's port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ('Socket created T1')

#managing error exe)ption
try:
    s.bind((HOST, PORT))
except socket.error:
    print ('Bind failed')

s.listen(1)
print ('Socket awaiting messages')
(conn, addr) = s.accept()
print ('connected')

#awaiting for messages
while True:
    try:
        dataEnc = conn.recv(1024)
    except socket.error:
        print('error recv')

    data = dataEnc.decode("utf-8")
    print ('I sent a message back in response to: ' + data)

    #process your message
    if data == 'Hello' :
        reply = 'Hi, back!'
    elif data == 'This is important':
        reply = 'Ok, I have done the important thing you have asked me!'

    #and so on and on until...
    elif data =='quit':
        conn.send('Terminate'.encode("utf-8"))
        break
    else:
        reply = 'Unknown command'

    # Sending (reply)
    conn.send(reply.encode("utf-8"))

print ('Closing communication')
conn.close()  # Close Connections
