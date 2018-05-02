#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	const char *ptr;
	char **pptr;
	struct hostent *hptr;
	char str[32]={'\0'};

	/*get the first arg ,which is the host name*/
	ptr=argv[1];// ex:www.baidu.com

	/*use get host by name */
	printf("get host %s\n",ptr);
	if((hptr=gethostbyname(ptr))==NULL){
		printf("get host error %s\n",ptr);
		return 0;
	}

	printf("official host name :%s\n", hptr->h_name);

	/*the host may have many names,print each of them*/
	
	for(pptr=hptr->h_aliases;*pptr!=NULL;pptr++){
		printf("alias:%s\n", (*pptr));
	}
	

	/*print it depend on the address types*/

	switch(hptr->h_addrtype){
		case AF_INET:
		case AF_INET6:
			pptr=hptr->h_addr_list;
			for(;*pptr!=NULL;pptr++){
				//printf("address:%s\n",inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
				printf("address:%s\n",inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			}
			printf("first address:%s\n",inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
			//printf("first address:%s\n",inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
			break;
		default:
			printf("unknown address type \n");
			break;
	}
	return 0;
}