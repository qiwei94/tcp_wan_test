import subprocess
import paramiko  
import time


def sshclient_execmd(hostname='128.104.222.126', port=22, username='qiwei94', password='1', execmd='ls'):  
    paramiko.util.log_to_file("paramiko.log")  
      
    s = paramiko.SSHClient()  
    s.set_missing_host_key_policy(paramiko.AutoAddPolicy())  
      
    s.connect(hostname=hostname, port=port, username=username, password=password)  
    stdin, stdout, stderr = s.exec_command (execmd)  
    stdin.write("Y")  # Generally speaking, the first connection, need a simple interaction.  
      
    print stdout.read()
      
    s.close()  
      
      
    
def main():  
    #sshclient_execmd(execmd='ls')  
    #sshclient_execmd(execmd='touch res_file_3333_100_100_0.txt')  
    sshclient_execmd(execmd='sudo ./server/us.o 3333 100 2 1000 eth0')

if __name__ == "__main__":  
    main()  