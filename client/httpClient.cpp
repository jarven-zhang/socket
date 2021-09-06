/*************************************************************************
#	> File Name:httpClient.cpp
#	> Author: jarven
#	> Mail: whuaw@aliyun.com
#	> Created Time: Tue 01 Jun 2021 05:36:07 PM CST
 ************************************************************************/

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "httpClient.h"

using namespace std;

HttpClient::HttpClient() 
{
	//此处一定要初始化一下，否则gethostbyname返回一直为空
//	WSADATA wsa = { 0 };
//	if(0 != WSAStartup(MAKEWORD(2, 2), &wsa)) 
//	{ //0 表示成功
//		cout << "WSAStartup error: " << WSAGetLastError() << endl;
//		return;
//	}
//	cout << "WSAStartup success: " << endl;
}

HttpClient::~HttpClient() 
{
	//清理winsock2的环境
//	WSACleanup();
}

void HttpClient::socketHttp(std::string host, std::string request) 
{
	sockaddr_in addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
	    cerr << "socket error!\n";
	    exit(-1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	inet_pton(AF_INET, host.c_str(), &addr.sin_addr);//ip地址转网络字节序

	if(-1 == connect(sockfd, (struct sockaddr *)&addr, sizeof(sockaddr_in))) 
	{
		cout << "connect error : " << endl;
		return;
	}

	send(sockfd, request.c_str(), request.size(), 0);

	cout << "client 发送数据: " << request.c_str() << endl;
	memset(_revcbuf, 0, sizeof(_revcbuf));

	//循环接收
	int offset = 0;
	int rc;
	while(rc = recv(sockfd, _revcbuf + offset, 1024, 0)) 
	{
		offset += rc;
	}

	close(sockfd);

	_revcbuf[offset] = 0;
	cout << "client 接收数据: " << _revcbuf << endl;
}

void HttpClient::postRequest(std::string host, std::string path, std::string post_content) 
{
	//POST请求方式
	std::stringstream stream;
	stream << "POST " << path;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Content-Type:application/x-www-form-urlencoded\r\n";
	stream << "Content-Length:" << post_content.length() << "\r\n";
	stream << "Connection:close\r\n\r\n";
	stream << post_content.c_str();

	socketHttp(host, stream.str());
}

void HttpClient::getRequest(std::string host, std::string path, std::string get_content) 
{
	//GET请求方式
	std::stringstream stream;
	stream << "GET " << path << "?" << get_content;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Connection:close\r\n\r\n";

	socketHttp(host, stream.str());
}


