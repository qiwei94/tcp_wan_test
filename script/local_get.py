#!/usr/bin/env python
#  TCP-Client
import socket
import sys
import threading
import time
from multiprocessing import Process

sk_obj=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sk_obj.connect(('127.0.0.1',8001))

while True:
    data = sk_obj.recv(8096)
    print 'Server send information : %s' % data.decode('utf-8')

sk_obj.close()
