#ifndef RMTOOL_H
#define RMTOOL_H
#include <string>
#include <vector>

class RmTool
{
public:
  RmTool();

  static int readConfigFile(const std::string& cfgfilepath, const std::string& key, std::vector<std::string>& value);
};

#endif // RMTOOL_H
