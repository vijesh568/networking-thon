#!/usr/bin/python

import socket
import struct
import threading 
import time
import Queue
import array

class ConnectionThread(threading.Thread):
	def __init__(self, threadID, sockID,queueID):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.sockID = sockID
		self.qID = queueID
		print "init called"
	def run(self):
		while True:
			self.buf = self.sockID.recv(8)
			self.num,self.len =struct.unpack('<II',self.buf)
			print "Connection Thread: self.num = " ,self.num
			print "Connection Thread: self.len = " ,self.len
			self.data=""
			if (self.num == 10):
				print "Data type is 10"
				self.buf=self.sockID.recv(self.len)
				dataobj = DataObject(10,0,self.len,self.buf)
			self.qID.put(dataobj)
			time.sleep(3)
	
class ParserThread(threading.Thread):
	def __init__(self,threadid,qID):
		threading.Thread.__init__(self)
		self.threadID = threadid
		self.qID = qID
		print "parser thread init called"
	def run(self):
		while True:
			mydata = self.qID.get()
			print "ParserTHread data:", +mydata.buf.tostring()
			print "ParserThread length: "+mydata.len
			time.sleep(3)

class DataObject():
	def __init__(self,type,stamp,len,data):
		self.type = type
		self.ts = stamp
		self.len = len
		self.buf = array.array('c')
		for ch in data:
			self.buf.append(ch)
		
		
	
if __name__ == "__main__":
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)
#	servername = raw_input("Enter the ip address of the server: ")
	s.connect(("192.168.56.101",12323));
	q = Queue.Queue()
	parsethread = ParserThread(2,q)
	parsethread.start()
	conthread = ConnectionThread(1,s,q)
	conthread.start()
	conthread.join()
	parsethread.join()
	s.close
