#!/usr/bin/python

import socket
import struct
from threading 
import time

class ReaderThread(threading.Thread):
	def __init__(self, threadID, sockID):
		self.threadID = threadID
		self.sockID = sockID
	def run(self):
		while True:
			self.buf = sockID.recv(1024)
			self.num,self.string,self.pad=struct.unpack('<I10sh',self.buf)
			print self.num
			print self.string
			time.delay(3)
	
	
if __name__ == "__main__":
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)
	servername = raw_input("Enter the ip address of the server: ")
	port = raw_input("Enter server port number: ")
	s.connect((servername,12323));
	rdthread = ReaderThread(1,s)
	s.close
