
/*************************************************************************
    > File Name: main.cpp
    > Author: jarven
    > Mail: whuaw@aliyun.com
    > Created Time: Thu 14 Jun 2018 11:17:53 AM CST
 ************************************************************************/
#include <iostream>

#include <glog/logging.h>
#include <sys/stat.h>
#include <algorithm>
//#include <unistd.h>
//#include <stdlib.h>
//#include <string.h>
#include <memory>

#include "type.h"
#include "HttpServer.h"

using namespace std;

auto main(int argc, char* argv[]) -> int{
    google::InitGoogleLogging(argv[0]); //初始化 glog

    mkdir(GLOG_PATH, 0777);
    google::SetLogDestination(google::GLOG_ERROR, GLOG_ERROR_FILE);
    google::SetLogDestination(google::GLOG_INFO,  GLOG_INFO_FILE);

    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_logbufsecs = 0;  //日志实时输出
    FLAGS_max_log_size = 10;
    FLAGS_stop_logging_if_full_disk = true;

    auto pServer = make_shared<HttpServer>();//shared_ptr<HttpServer> pServer = make_shared<HttpServer>();
    if(0 == pServer->init()){
        pServer->start();
    }

    return 0;
}
