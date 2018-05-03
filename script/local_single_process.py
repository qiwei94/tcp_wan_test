#!/usr/bin/env python
#  TCP-Client
import socket
import sys
import threading
import time
from multiprocessing import Process

sk_obj=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sk_obj.connect(('127.0.0.1',8001))
sk_obj.setblocking(0)

while True:
    msg = raw_input('-->').strip()
    if len(msg)==0:
        try:
            data = sk_obj.recv(8096)
        except socket.error:
            print "no word get"
            continue
        else:
            print data.decode('utf-8')
            continue
    sk_obj.send(msg.encode('utf-8'))

#t.join()
sk_obj.close()
