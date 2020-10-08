#include "rmtool.h"
#include "glog/logging.h"
#include <fstream>
using namespace std;

const int BUFF_SIZE = 100;

RmTool::RmTool()
{
}

/*
 *parameter: cfgfilepath 文件的绝对路径名如: /work/server/my.cfg
 *           key         文本中的变量名
 *           value       对应变量的值，用于保存
 */
int RmTool::readConfigFile(const string& cfgfilepath, const string& key, vector<string>& value)
{
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//打开文件
    if( ! cfgFile.is_open())
    {
        LOG(ERROR)<<"can not open the cfg file: "<< cfgfilepath << endl;
        return -1;
    }

    char tmp[BUFF_SIZE];
	LOG(INFO) << "-----------RmTool.cpp------------" ;
	LOG(INFO) << "cfg file:" << cfgfilepath;
    while(!cfgFile.eof())//循环读取每一行
    {
        cfgFile.getline(tmp, BUFF_SIZE);//每行读取前100个字符
        string line(tmp);
		LOG(INFO) << "Line data:" << line;
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
        if(pos == string::npos)
        {
            continue;
        }

        string tmpKey = line.substr(0, pos);//取=号之前
        if(key == tmpKey)
        {
            value.push_back(line.substr(pos + 1));//取=号之后
        }
    }

    return 0;
}

/*
Paramters:
1.file : IP白名单
2.value: 传出参数，将白名单中的ip存储在vector中

定义一个file文件（随便什么格式文件），内容如下：
127.0.0.1
127.0.0.2
172.0.0.1
*/
int RmTool::getIpWhitelist(const string& file, vector<string>& value)
{
	fstream cfgFile;
    cfgFile.open(file);//打开文件
    if( ! cfgFile.is_open())
    {   
        LOG(ERROR)<<"can not open the cfg file: "<< file << endl;
        return -1; 
    }   
 
    char tmp[BUFF_SIZE];
    while(cfgFile.getline(tmp, BUFF_SIZE))//循环读取每一行
    {   
		value.emplace_back(tmp);
	}
	return 0;
}
