#!/usr/bin/python

import socket
import struct

if __name__ == "__main__":
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)
	servername = raw_input("Enter the ip address of the server: ")
	port = raw_input("Enter server port number: ")
	s.connect((servername,12323));
	buf= s.recv(1024)
	print "Received buffer size is "+len(buf)
	num,string,pad=struct.unpack('<I10sh',buf)
	print num
	print string
	s.close
