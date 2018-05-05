import sys
import os
import time
import re

import subprocess
from multiprocessing import Process
import multiprocessing
from subprocess import Popen,PIPE
import sys 

#user_ip='128.104.222.126'
#user_name="qiwei94"
#paasswd="1"


#user_ip='101.200.181.242'
#user_name="root"
#paasswd="8266Cs.123"
#server_work_cwd='/home'
#local_work_cwd='/home/c/tcp_wan_test/unity'
#front_name=''


user_ip='128.104.222.140'
user_name="qiwei94"
paasswd='1'
server_work_cwd='/users/qiwei94/server'
local_work_cwd='/home/c/tcp_wan_test/unity'
front_name='bbr'


convenient_privilege=" echo "+paasswd+"| "
link_cmd=" ssh -t "+user_name+"@"+user_ip


def ssh_link():
    result=os.system("%s sudo -S ls" % link_cmd)
    print "result="
    print result


def run_test(ip=user_ip,\
			port=2000,\
			client_conn_interval=10000,\
			res_name='client_res',\
			conn_num=1000,\
			conn_times=1,\
			send_interval=0,\
			eth_port="eth0"):

	remote_cmd=link_cmd+" sudo -S ./server/us.o "+str(port)+" "+str(conn_num)+" "+str(conn_times)+" "+str(send_interval)+" "+str(eth_port)+" "+str(front_name)+" "
	local_cmd="./uc.o 0 "+str(ip)+" "+str(port)+" "+str(conn_num)+" "+str(client_conn_interval)+" > "+res_name

	print remote_cmd
	print local_cmd
	
	remote=subprocess.Popen(remote_cmd,shell=True)
	time.sleep(10)
	local=subprocess.Popen(local_cmd,shell=True,cwd="/home/c/tcp_wan_test/unity",stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	remote.wait();
	local.wait()
	print "both finished bye...."



def main():
	port_seq=[3000,4000,5000]
	conn_num_seq=[1,500,1000]
	conn_times_seq=[1000,2,1]
	#port_seq=[6000]
	#conn_num_seq=[1000]
	#conn_times_seq=[1]

	for i in range(3):
		port_basic=port_seq[i]
		conn_num=conn_num_seq[i]
		conn_times=conn_times_seq[i]
		for j in range(10):
			port=port_basic+j
			run_test(port=port,conn_num=conn_num,conn_times=conn_times)
			time.sleep(10)


if __name__ == '__main__':
	main()




#remot_cmd=link_cmd+" sudo -S ./server/us.o 3362 100 2 1000 eth0 "
#	local_cmd="./uc.o 0 128.104.222.126 3362 100 1000 > client_res "
#	"""
#	#remote=subprocess.Popen(remot_cmd,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
#	#time.sleep(20)
#	#local=subprocess.Popen(local_cmd,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
#	"""
#	remote=subprocess.Popen(remot_cmd,shell=True)
#	time.sleep(10)
#	local=subprocess.Popen(local_cmd,shell=True,cwd="/home/c/tcp_wan_test/unity",stdout=subprocess.PIPE,stderr=subprocess.PIPE)
#	remote.wait();
#	local.wait()
#	print "both finished bye...."

