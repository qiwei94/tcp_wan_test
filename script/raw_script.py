import sys
import os
import time
import re

from subprocess import Popen,PIPE

user_name="root"
paasswd="1"
#user_ip="192.168.50.61"
user_ip="127.0.0.1"

convenient_privilege=" echo "+paasswd+"|"
link_cmd="ssh -t "+user_name+"@"+user_ip


def ssh_link():
    result=os.system("%s sudo -S ls" % link_cmd)
    print "result="
    print result

def show_qdisc(dev="s1-eth1",detail=0):
	print dev+" qdisc is ::::::::::::::::::::::::::::::::::::::::::::::::::::"
	if detail:
		os.system("%s sudo -S tc -s qdisc show dev %s " % link_cmd,dev)
	else:
		os.system("%s sudo -S tc qdisc show dev %s " % link_cmd,dev)




ssh_link()
show_qdisc(dev='eth0')