#!/usr/bin/python

import socket
import struct
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)

s.connect((socket.gethostname(),12323));
buf= s.recv(1024)
print len(buf)
num,string,pad=struct.unpack('<I10sh',buf)
print num
print string
s.close
