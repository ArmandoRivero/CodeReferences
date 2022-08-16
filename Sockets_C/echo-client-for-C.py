#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Author: Armando Rivero
# Date:   2019.07.26
# Modif:
# Ver.:   1.0
# =============================================================================

#!/usr/bin/env python3

import socket

#HOST = '127.0.0.1'  # The server's hostname or IP address
HOST = '192.168.100.99'  # The server's hostname or IP address
PORT = 65432        # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Hello, world')
    data = s.recv(1024)

print('Received', repr(data))
