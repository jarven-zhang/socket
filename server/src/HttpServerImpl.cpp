/**
 * Project Untitled
 */

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <algorithm>
#include <sys/epoll.h>
#include "HttpServerImpl.h"
#include "rmtool.h"
#include "glog/logging.h"
#include "type.h"

#define MAX_EVENTS 10
#define W_LIST 0
/**
 * HttpServerImpl implementation
 */

using namespace std;

const int QUEUE_MAX_COUNT = 5;
const int RECEIVE_BUFF_SIZE = 526;
const int SEND_BUFF_SIZE = 526;

HttpServerImpl::HttpServerImpl(){}

HttpServerImpl::~HttpServerImpl(){}

int HttpServerImpl::init() 
{
    struct sockaddr_in server_addr;

    string ip_file = PROOT_PATH;
    ip_file += IP_PATH;

	//获取IP白名单
    if(-1 == RmTool::getIpWhitelist(ip_file, ip_list))
    {
        LOG(ERROR) << "read configure file error!";
        exit(-1);
    }

    //创建socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd)
    {
        LOG(ERROR) << "socket error!";
        exit(-1);
    }
    memset(&server_addr, 0, sizeof(server_addr));

    //设置端口，IP，和TCP/IP协议族
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(HTTP_PORT) ;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;

    //套接字关闭后, 套接字状态TIME_WAIT约保留2到4分钟, 为了不bind失败，设置允许重用
    int on = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    //绑定
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))  < 0)
    {
        LOG(ERROR) << "bind error!";

        close(listen_fd);
        listen_fd = -1;
        exit(-1) ;
    }

    //监听
    if (listen(listen_fd, QUEUE_MAX_COUNT) < 0)
    {
        LOG(ERROR) << "listen error!";
        close(listen_fd);
        listen_fd = -1;
        exit(-1) ;
    }

    LOG(INFO) << "http server running on port :" << HTTP_PORT;

    return 0;
}

int HttpServerImpl::start() 
{
    struct sockaddr_in client_addr;
	socklen_t clientAddrLen = sizeof(client_addr);

    char recv_buf[RECEIVE_BUFF_SIZE];

    string send_buf = "[Receive]:";

	int nfds = 0;
	struct epoll_event ev;//作为监听fd的epoll事件
	struct epoll_event events[MAX_EVENTS];//作为epoll_wait的事件列表返回数组

	int epollfd = epoll_create(10);
	if (epollfd < 0)
	{
		LOG(ERROR) << "Epoll create error!";
		exit(-1);
	}

	//添加监听fd
	ev.events = EPOLLIN;
	ev.data.fd = listen_fd;
	//这些代码都可以直接通过man epoll来获取，里面有示例
	if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_fd, &ev))
	{
    	perror("epoll_ctl: listen_sock");
    	exit(EXIT_FAILURE);
 	}

	cout << "----------------listen fd: " << listen_fd << endl;

	//创建一个临时变量存放Fd
	int tmp_fd = -1;
    while (1)
    {
		//epoll_wait阻塞监听消息, 返回值为有事件的fd的数量
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);//参数-1表示阻塞
		if (nfds < 0)
		{
    		perror("epoll_pwait");
		    exit(EXIT_FAILURE);
		}

		//开始遍历所有fd，查找有消息的fd
		for(int n = 0; n < nfds; ++n)
		{
			tmp_fd = events[n].data.fd;
            cout << "Get a fd:" << tmp_fd << endl;

			//如果fd等于listen fd说明是有新的连接上来
			if (listen_fd == tmp_fd)
			{
				int client_fd = accept(tmp_fd, (sockaddr*)&client_addr, &clientAddrLen);
				if (client_fd < 0)
				{
					LOG(ERROR) << "accept error!";
					continue;
				}

				RmTool::setnonblocking(client_fd);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = client_fd;
				if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev))
				{
                	perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }

            	string client_address = inet_ntoa(client_addr.sin_addr);
                LOG(INFO) << "accept a client ! ip:" << client_address.c_str();
        
#if W_LIST
        		//判断这个客户端IP是在白名单中的
        		if(!ipInWhitelist(client_address))
                {
					const char *errorString = "You can not connet to it!";
                    LOG(ERROR) << "!!!!Unknow client:" << client_address.c_str() << ", close the connector!";
                	send(client_fd, errorString, strlen(errorString) + 1, 0);
                    close(client_fd);
                    break;
                }
#endif
			}
			else//说明此fd是已经连接上来且开始传输数据
			{
    			memset(recv_buf, 0, RECEIVE_BUFF_SIZE);
    			send_buf.clear();

                // Get the data send by client
        		// 接收缓冲区recv_buf，该缓冲区用来存放recv函数接收到的数据
                if(0 > recv(tmp_fd, recv_buf, RECEIVE_BUFF_SIZE, 0))
                {
                    LOG(ERROR) << "receive error! Maybe the connect is off!";
                    close(tmp_fd);
                    break;
                }

                LOG(INFO) << "Recv client's data:" << recv_buf;
	            // 发送响应给客户端
            	//sendToClient(client_fd, send_buf, strlen(send_buf) + 1);
				send_buf += recv_buf;
            	send(tmp_fd, send_buf.c_str(), send_buf.size() + 1, 0);
			}
		}//for end
    }//while end
    close(listen_fd) ;

    return 0;
}

int HttpServerImpl::sendToClient(int& client_fd, const char *send_buf, int buf_size)
{
    char sendBuff[buf_size];
    memset(sendBuff, 0, buf_size);

#if 0
    send(client_fd, HTTP_HEAD, strlen(HTTP_HEAD), 0);
    send(client_fd, SERVER_STRING, strlen(SERVER_STRING),0);
    send(client_fd, CONTENT_TYPE, strlen(CONTENT_TYPE), 0);
    send(client_fd, "\r\n", strlen("\r\n"), 0);
#endif
    memcpy(sendBuff, send_buf, strlen(send_buf));
    LOG(INFO) << "####### send to client:[" << sendBuff<< "]" << endl;
    send(client_fd, sendBuff, buf_size - 1, 0);

    return 0;
}

int HttpServerImpl::handle() 
{
    //todo
    return 0;
}
