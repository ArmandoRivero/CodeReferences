#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

# author: Armando Rivero
# date:   2019.05.21
# modif.: 2019.05.21
# ver.:   1.0

# Use this file in the case the compiled file is located
# inside another folder

import subprocess
from subprocess import Popen, PIPE

Popen(['./SerialLogger/ && Make'])
Popen(['./TDC7200_ToF/ && Make'])
