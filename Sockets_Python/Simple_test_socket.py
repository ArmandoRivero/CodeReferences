#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Author: Armando Rivero
# Date:   2019.07.02
# Modif:  2019.12.03
# Ver.:   1.00
# =============================================================================
import sys
import datetime
import os
import csv
import time
import socket

import numpy as np


HOST = '192.168.1.111' #Enter IP or Hostname of your server
#HOST = '10.11.15.185' #Enter IP or Hostname of your server
PORT = 65432

# open socket communication
s_rpi1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s_rpi1.connect((HOST,PORT))

# Send command to server
command = \
        'FromMac.'

print('  -> socket cmd: {}'.format(command))
s_rpi1.send(command.encode("utf-8"))
