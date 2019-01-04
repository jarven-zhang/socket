
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
#if 1
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

//    google::ShutdownGoogleLogging();
#else
//    vector<string> va ;
//    string client_address = "aaaa";

//    va.push_back("11111");
//    va.push_back("22222");
//    va.push_back("33333");
//    auto iter = find(va.begin(), va.end(), client_address);
//    if(iter == va.end())
//    {
//        LOG(ERROR) << "!!!!Unknow clie.1`nt:" << client_address.c_str() << ", close the connector!";
////        close(client_fd);
////        continue;
//    }


    vector<int> vec;

        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        vec.push_back(4);
        vec.push_back(5);
        vec.push_back(6);

        vector<int>::iterator it = find(vec.begin(), vec.end(), 6);

        if (it != vec.end())
            cout<<*it<<endl;
        else
            cout<<"can not find"<<endl;


#endif
    return 0;
}
