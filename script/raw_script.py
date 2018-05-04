import sys
import os
import time
import re

import subprocess
from multiprocessing import Process
import multiprocessing
from subprocess import Popen,PIPE
import sys 


user_name="qiwei94"
paasswd="1"
#user_ip="192.168.50.61"
user_ip='128.104.222.126'

convenient_privilege=" echo "+paasswd+"|"
link_cmd="ssh -t "+user_name+"@"+user_ip


def ssh_link():
    result=os.system("%s sudo -S ls" % link_cmd)
    print "result="
    print result

def local_client():
	os.system("./home/c/tcp_wan_test/unity/uc.o 0 128.104.222.126 3316 100 1000")

def remote_server():
	os.system("%s sudo -S ./server/us.o 3316 100 2 1000 eth0 " % link_cmd)



def main():
	remot_cmd=link_cmd+" sudo -S ./server/us.o 3360 100 2 1000 eth0 "
	local_cmd="./uc.o 0 128.104.222.126 3360 100 1000"
	"""
	remote=subprocess.Popen(remot_cmd,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	time.sleep(20)
	local=subprocess.Popen(local_cmd,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	"""

	remote=subprocess.Popen(remot_cmd,shell=True)
	
	time.sleep(10)
	
	local=subprocess.Popen(local_cmd,shell=True,cwd="/home/c/tcp_wan_test/unity",stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	
	
	while True:
		local.stdout.flush()
    	print "local::::::",local.stdout.readlines()
    	time.sleep(1)
	
#ssh -t qiwei94@128.104.222.126 sudo -S ./server/us.o 3333 100 2 1000 eth0
	


if __name__ == '__main__':
	main()