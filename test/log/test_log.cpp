#include "public/log_file.h"

int main(int argc, char const *argv[])
{
    std::string name = "boofish";
    int cnt = 0;
    while(true)
    {
        LOG_SHOW(LOGI_TYPE::LOGI_ERROR, "错误日志测试 %d %s\n", 234, name.c_str());
        LOG_SHOW(LOGI_TYPE::LOGI_WARN, "警告日志测试 %d %s\n", 234, name.c_str());
        LOG_SHOW(LOGI_TYPE::LOG_INFO, "信息日志测试 %d %s\n", 234, name.c_str());
        LOG_SHOW(LOGI_TYPE::LOGI_DEBUG, "调试日志测试 %d %s\n", 234, name.c_str());

        
        LOG_FILE(LOGI_TYPE::LOGI_ERROR, "错误日志测试 %d %s\n", 234, name.c_str());
        LOG_FILE(LOGI_TYPE::LOGI_WARN, "警告日志测试 %d %s\n", 234, name.c_str());
        LOG_FILE(LOGI_TYPE::LOG_INFO, "信息日志测试 %d %s\n", 234, name.c_str());
        LOG_FILE(LOGI_TYPE::LOGI_DEBUG, "调试日志测试 %d %s\n", 234, name.c_str());   

        sleep(2);
        cnt ++;
        if(cnt > 100)
        {
            break;
        }
    }
 
    return 0;
}
