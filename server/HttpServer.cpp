/**
 * Project Untitled
 */

#include <iostream>
#include "HttpServer.h"

#include "type.h"
#include "rmtool.h"

/**
 * HttpServer implementation
 */

using namespace std;

HttpServer::HttpServer(){
  httpServerImpl = new HttpServerImpl();
}

HttpServer::~HttpServer(){
    delete httpServerImpl;
    httpServerImpl = nullptr;
}

int HttpServer::init() {
  return httpServerImpl->init();
}

int HttpServer::start() {
//    LOG(INFO) << __func__ << endl;
    return httpServerImpl->start();
}
