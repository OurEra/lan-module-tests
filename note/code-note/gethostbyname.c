//域名解析

#include<stdio.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

/*  #include<netdb.h>
struct hostent
{
	char *h_name;		//主机名,cn.l.google.com
	char *h_aliases;	//主机的别名,如www.google.com
	int h_addrtype;		//主机地址族类型,AF_INET或AF_INET6
	int h_length;		//返回地址长度,IPv4的长度为4,IPv6为16
	char **h_addr_list;	//返回主机的地址列表,以网络字节序存储
};
#define h_addr h_addr_list[0]

struct hostent *gethostbyname(const char *hostname);	
//gethostname()通过 "/etc/resolv.conf"里的DNS主机列表向DNS服务器发送UDP查询
//或通过本地静态主机文件"/etc/hosts"为实现域名解析
extern int h_errno;
char *hstrerror(int err);
*/


int main(int argc,char **argv)
{
	char **pptr;
	char str[46];
	struct hostent *hptr;
	if(argc<2)
	{
		fprintf(stderr,"usage:domain<domain>\n");
		return -1;
	}
	//当gethostbyname()成功执行将返回非空的hostent结构地址,失败则返回空指针,并用错误码设置全局变量h_errno,
	//可以通过调用hstrerror()并传递h_errno作为参数取得错误描述信息
	if((hptr=gethostbyname(argv[1]))==NULL) 
	
	{
		fprintf(stderr,"gethostbyname call failed.%s\n",hstrerror(h_errno));
		return -1;
	}
	printf("official name: %s\n",hptr->h_name);//显示主机名
	for(pptr=hptr->h_aliases;*pptr!=NULL;pptr++)//显示所有的主机别名
	{
		printf("\t alias: %s\n",*pptr);
	}
	if(hptr->h_addrtype!=AF_INET)
	{
		fprintf(stderr,"Invalid address type %d\n",hptr->h_addrtype);
		return -1;
	}
	pptr=hptr->h_addr_list;//显示IP地址列表
	for(;*pptr!=NULL;pptr++)
	{
		printf("\t address: %s\n",inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
	}
	return 0;
}
