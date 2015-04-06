#!/usr/bin/python

import socket
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)

s.connect((socket.gethostname(),12323));
print s.recv(1024)
s.close
