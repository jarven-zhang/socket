#pragma once
#include <iostream>

using namespace std;

class HttpClient 
{
public:
	HttpClient();
	~HttpClient();

	void socketHttp(std::string host, std::string request);

	void postRequest(std::string host, std::string path, std::string post_content);
	void getRequest(std::string host, std::string path, std::string get_content);


private: 
	unsigned short _port = 8089; //端口
	char _revcbuf[1024 * 3]; 

};

