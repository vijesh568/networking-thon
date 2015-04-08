#!/usr/bin/python

import socket
import struct
import threading 
import time
import queue

class ReaderThread(threading.Thread):
	def __init__(self, threadID, sockID,queueID):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.sockID = sockID
		self.qID = queueID
		print "init called"
	def run(self):
		while True:
			self.buf = self.sockID.recv(1024)
			self.num,self.string,self.pad=struct.unpack('<I10sh',self.buf)
			q
			time.sleep(3)
	
	
if __name__ == "__main__":
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)
	servername = raw_input("Enter the ip address of the server: ")
	s.connect((servername,12323));
	q = queue.Queue()
	rdthread = ReaderThread(1,s,q)
	rdthread.start()
	rdthread.join()
	s.close
