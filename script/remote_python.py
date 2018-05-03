#!/usr/bin/env python
#  TCP-Server
import socket
import subprocess
from multiprocessing import Process
import multiprocessing
import time



sk_obj = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sk_obj.bind(('127.0.0.1',8001))
sk_obj.listen(5)

p_list=[]

def task(from_recv,conn):
    #res = subprocess.Popen(from_recv.decode('utf-8'),shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    res = subprocess.Popen(from_recv.decode('utf-8'),shell=True,close_fds=True)
    msg = res.stdout.read()
    if len(msg) == 0:
        msg = res.stderr.read()
        print msg
        conn.send(msg)


def task_kill_all():
    num = len(p_list)
    print "p_list len  is ",num
    for i in range(num):
        p=p_list[i]
        if p.is_alive():
            print  "Process:"+ p.pid+" is running, and will be killed"
            p.terminate() 
        else:
            print "it is already died"


while True:
    conn,ipaddr = sk_obj.accept()
    print ('connection from ip: %s' % ipaddr[0])
    count = 0
    while True:
        try:
            #print "try..."
            new_msg="now give me "+str(count)+" command"
            #print "asfdasf"
            conn.send(new_msg)
            from_recv = conn.recv(8096)
            if len(from_recv) == 0:continue
            print ('from ip : %s information : %s' % (ipaddr[0],from_recv))
            if from_recv.decode('utf-8')=='work begin':
                print "need to begin"
                conn.send("work begin")
                break
            name=from_recv.decode('utf-8')
            p = Process(target=task, args=(from_recv,conn),name=name)
            #p.start()
            #print from_recv.decode('utf-8')," and then???"
            p_list.append(p)
            #print from_recv.decode('utf-8')," fork success, it is ",count
            count+=1

        except Exception:
            break
    print "begin all the process"
    for i in range(len(p_list)):
        print i," begin:"
        p=p_list[i]
        p.start()
        #time.sleep(1)

    for p in multiprocessing.active_children():
        print("child p.name: " + p.name + "\tp.id: " + str(p.pid))

    while True:
        from_recv = conn.recv(8096)
        if from_recv.decode('utf-8')=='work exit':
            print "need to exit"
            task_kill_all()
            conn.close()
            break
        time.sleep(1)

sk_obj.close()

print "finish"