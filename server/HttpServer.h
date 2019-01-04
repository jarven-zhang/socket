/**
 * Project Untitled
 */


#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

#include "HttpServerImpl.h"

class HttpServer {
public: 
  HttpServer();
  ~HttpServer();
  int init();

  int start();

private: 
  HttpServerImpl *httpServerImpl;
};

#endif //_HTTPSERVER_H
