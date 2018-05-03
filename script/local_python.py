#!/usr/bin/env python
#  TCP-Client
import socket
import sys
import threading
import time
from multiprocessing import Process

sk_obj=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sk_obj.connect(('127.0.0.1',8001))

'''
def get_msg():
	while True:
		#print "work??"
		data = sk_obj.recv(8096)
    	print 'Server send information : %s' % data.decode('utf-8')
    	#time.sleep(0.5)

#t =threading.Thread(target=get_msg)
#t.start()
p_get = Process(target=get_msg,name='get msg')
p_get.start()
'''
while True:
    msg = raw_input('-->').strip()
    if len(msg)==0:continue
    sk_obj.send(msg.encode('utf-8'))
    data = sk_obj.recv(8096)
    print 'Server send information : %s' % data.decode('utf-8')

#t.join()
sk_obj.close()
