
/*

int sockfd(int domain,int type,int protocol)
//domain 固定AF_INET 代表IPv4
//type有2个参数
//SOCK_DGRAM   对应UDP
//SOCK_STREAM  对应TCP
//protocol 固定0为系统自动


int getsockopt(int sockfd,int level,int optname,void *optval,socklen_t *optlen)
int setsockopt(int sockfd,int level,int optname,const void *optval,socklen_t optlen)
 
//level的值是SOL_SOCKET , IPPROTO_IP , IPPROTO_TCP (后2个仅限研发TCPIP的超级工程师调试时用)

//optname的值是 SOL_SOCKET的选项
//SO_BROADCAST	广播
//SO_REUSEADDR	充许重复使用本地址	,
//		在TCP态时,因为TCP不允许2个进程调用bind()捆绑相同的地址和端口,可能会操作不成功,
//		在TCP态时,用LINGER解决ReUserAddr的问题
//SO_LINGER	如果有未确认的数据,是否推迟关闭套接字
//SO_RCVBUF	套接字 接收缓冲区 大小,用optval设置其int值
//SO_SNDBUF	套接字 发送缓冲区 大小,用optval设置其int值

//SO_LINGER是结构体
//#include<sys/socket.h>
//struct linger
//{
//	int l_onoff;	// 0=关闭, 非0=打开
//	int l_linger;	//等待时间(秒)
//};

*/

#include<sys/socket.h>
//一些应用例子	
	int opt;

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)//创建UDP套接字
	{
		herror("ERROR opening socket");
		return RET_ERR;
	}
	
	if((getsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,&opt,sizeof(opt)))<0)//获取套接字的 发送缓冲大小,大小数值放到opt
	{
		perror("ERROR setsockopt");
		return RET_ERR;
	}

	if((setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&opt,sizeof(opt)))<0)//以opt数值 设置套接字 接收缓冲大小,
	{
		perror("ERROR setsockopt");
		return RET_ERR;
	}

	if((setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)))<0)//设为SO_BROADCAST广播模式
	{
		perror("ERROR setsockopt");
		return RET_ERR;
	}

	if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)))<0)//设为 允许重用本地址
	{
		perror("ERROR setsockopt");
		return RET_ERR;
	}

	
	////
	struct linger so_linger;
	so_linger.l_onoff=1; //打开确认推迟发送
	so_linger.l_linger=0;//等待时间为0, 因为等待时间为0,所以TCP直接丢弃未发送和未确认的数据
	if((setsockopt(sockfd,SOL_SOCKET,SO_LINGER,&so_linger,sizeof(so_linger)))<0)//设为 允许重用本地址
	{
		perror("ERROR setsockopt");
		return RET_ERR;
	}
	
	////







