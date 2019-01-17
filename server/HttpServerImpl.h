/**
 * Project Untitled
 */


#ifndef _HTTPSERVERIMPL_H
#define _HTTPSERVERIMPL_H

#include "Handler.h"
#include <vector>
#include <string>

class HttpServerImpl 
{
public: 
    
    HttpServerImpl();

    int init();

    int start();

    ~HttpServerImpl();

private: 

    int server_fd;

    std::vector<std::string> ip_list;

    Handler hander;
    
    int sendToClient(int& client_fd, const char *send_buf, int buf_size);
    
    int handle();
};

#endif //_HTTPSERVERIMPL_H
