from __future__ import division  
import sys
import os
import time
import re

import subprocess
from multiprocessing import Process
import multiprocessing
from subprocess import Popen,PIPE
import sys 


def get_file(file_dir):
	for root, dirs, files in os.walk(file_dir):  
		print(root)
		print(dirs) 
		print(files)


def process(file_dir):
	cmd1 = 'tshark -r '
	cmd3 = ' -Y \'tcp.ack>1\' -T fields -e frame.time_relative '
	cmd_head = '| head -n 1 '
	cmd_tail = '| tail -n 1 '
	for root, dirs, files in os.walk(file_dir):
		pass
	packet_loss_cmd1 = 'tshark -r '
	packet_loss_cmd3 =' -Y \'tcp.analysis.lost_segment || tcp.analysis.retransmission || tcp.analysis.duplicate_ack\' |wc -l'

	total_num_cmd1 = 'tshark -r '
	total_num_tail = ' -T fields -e frame.number | tail -n 1 '

	for file in files:
		print file,":"
		
		cmd=total_num_cmd1+str(file)+total_num_tail
		total=subprocess.Popen(cmd,shell=True,cwd=file_dir,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		total_num = total.stdout.readline()  
		total.wait()
		#print total_num

		cmd=packet_loss_cmd1+str(file)+packet_loss_cmd3
		loss=subprocess.Popen(cmd,shell=True,cwd=file_dir,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		loss_num = loss.stdout.readline()  
		loss.wait()
		#print loss_num

		pack_loss_rato=float(float(loss_num)/float(total_num))
		print pack_loss_rato

		cmd = cmd1+str(file)+cmd3+cmd_head
		head_res=subprocess.Popen(cmd,shell=True,cwd=file_dir,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		head_time = head_res.stdout.readline()  
		head_res.wait()
		#print head_time

		cmd = cmd1+str(file)+cmd3+cmd_tail
		tail_res=subprocess.Popen(cmd,shell=True,cwd=file_dir,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		tail_time = tail_res.stdout.readline()  
		tail_res.wait()
		#print tail_time

		delta_time = float(tail_time)-float(head_time)
		print delta_time

		print "............................................."

def main():
	process('/home/c/tcp_wan_test/res/res')


if __name__ == '__main__':
	main()




