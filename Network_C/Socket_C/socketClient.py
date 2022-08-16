#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Author: Armando Rivero
# Date:   2021.04.13
# Modif:
# Ver.:   1.0
# =============================================================================

# The commands to send must have a '.' at the end
# USe this comand to test: shootImgAcqu.1.30.
# The 'C' file associated is "run_slave.c"

import socket
import time


HOST = '10.42.0.1'#Enter IP or Hostname of your server
PORT = 65432 # Pick an open Port (1000+ recomended) , must match the server port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#Lets loop awaiting for your input
imgQqtty_cnt = 0
while True:
    command = input('Enter command: ')

    print('-> Sending command')
    print('  -> socket cmd: {}'.format(command))

    if command == 'exit':
    	s.close()
    	print('socket closed!')
    	break

    s.send(command.encode("utf-8"))
    # reply = s.recv(1024)
    # if reply.decode('utf-8')=='client quit':
    #     break
    timeoutCnt = 0
    while True:
	    reply = s.recv(1024)
	    print ('    <- {}'.format(reply))

	    answerToTokenize = reply.decode('utf-8')
	    dataTokensStrings = answerToTokenize.split('\r\n')
	    #print('    -> dataTokensStrings: {}'.format(dataTokensStrings))
	    print('    -> Message lenght = {}'. format(len(dataTokensStrings)))
	    print('    -> dataTokensStrings[0]: {}'.format(dataTokensStrings[0]))

	    #if (dataTokensStrings == 'acquisition Finished\r\n'):
	    if (dataTokensStrings[0] == 'acquisition Finished'):
	        ack = 'Ok\n\r'
	        s.send(ack.encode("utf-8"))
	        print('    -> Finish acquisition')
	        break
	    elif (timeoutCnt >= 10):
	        print('Timeout waiting answer')
	        exitProgramFlag = 1;
	        break;

	    else:
	        dataTokens = dataTokensStrings[0]
	        print('    -> Iteration:{} - dataTokens: {}'.format(imgQqtty_cnt,dataTokens))
	        imgQqtty_cnt = imgQqtty_cnt + 1

	        # separate data to extract results
	        dataTokens = dataTokensStrings[0].split(',', 4)
	        tok_1 = dataTokens[0]
	        tok_2 = dataTokens[1]
	        tok_3 = dataTokens[2]
	        tok_4 = dataTokens[3]

	        print('-> Received data: \n\ttok_1={} \n\ttok_2={} \n\ttok_3={} \n\ttok_4={}'.format(tok_1, tok_2, tok_3, tok_4))

	    print('delay');
	    time.sleep(0.01)
	    timeoutCnt = timeoutCnt +1




    print('  -> answer from server:')
    print (reply.decode("utf-8"))

print ('Closing communication')

s.close()

