#include "rmtool.h"
#include "glog/logging.h"
#include <fstream>
using namespace std;

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

    char tmp[100];
    while(!cfgFile.eof())//循环读取每一行
    {
        cfgFile.getline(tmp,100);//每行读取前1000个字符，1000个应该足够了
        string line(tmp);
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
