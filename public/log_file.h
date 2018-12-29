#pragma once
/*
名称:日志文件系统
说明:必须手动增加\n
*/
#include <stdarg.h>
#include <memory>
#include "singleton.h"
#include "log_file_io.h"

enum class LOGI_TYPE
{
    LOGI_DEBUG=0,
    LOG_INFO,
    LOGI_WARN,
    LOGI_ERROR,
};

class log_file
{
public:
    log_file(std::string dir="log"):debug_(dir, get_process_name(), "debug"),warn_(dir, get_process_name(), "warn"),error_(dir, get_process_name(), "error"),log_lvl_(LOGI_TYPE::LOGI_DEBUG){ creat_dir(dir); }
    
    //日志
    inline void write_log(LOGI_TYPE log_lvl, const char *contxt, unsigned int length);
    //日志
    inline void write_log(LOGI_TYPE log_lvl, std::string file, int line, const char *format, ...);
    //显示日志
    inline void show_log(LOGI_TYPE log_lvl, const char *format, ...);
    
private:
    inline void creat_dir(std::string dir);

private:
    const int MAX_FILE_LEN = 1024 * 2;
    log_file_io debug_;
    log_file_io warn_;
    log_file_io error_;
    LOGI_TYPE log_lvl_;
};

//
//line
//
void log_file::creat_dir(std::string dir)
{
    if(0 != access(dir.c_str(), F_OK))
    {
        if(0 != mkdir(dir.c_str(), 0x777))
        {
            printf("create dir %s\n", strerror(errno));
        }
    }
}

void log_file::write_log(LOGI_TYPE log_lvl, const char *contxt, unsigned int length)
{
    if(log_lvl >= log_lvl_)
    {
        switch(log_lvl)
        {
            case LOGI_TYPE::LOGI_DEBUG:
            case LOGI_TYPE::LOG_INFO:
                debug_.lwrite(contxt, length);
            case LOGI_TYPE::LOGI_WARN:
                warn_.lwrite(contxt, length);
            case LOGI_TYPE::LOGI_ERROR:
                error_.lwrite(contxt, length);
            default:
            break;
        }
    }
}

void log_file::write_log(LOGI_TYPE log_lvl, std::string file, int line, const char *format, ...)
{
    if(log_lvl >= log_lvl_)
    {
        char txt_buf[MAX_FILE_LEN+1] = {'\0'};
        std::string header = get_process_name() + " " + get_time_for_daytime() + " " + file+ ":" + std::to_string(line) + " ";
        memcpy(txt_buf, header.c_str(), header.length());
        
        va_list v_arg;
        va_start (v_arg, format);
        unsigned int len = vsnprintf(txt_buf+header.length(), MAX_FILE_LEN-header.length(), format, v_arg);
        va_end(v_arg);   

        write_log(log_lvl, txt_buf, len+header.length());     
    }
}

void log_file::show_log(LOGI_TYPE log_lvl, const char *format, ...)
{
    if(log_lvl >= log_lvl_)
    {
        std::string color;
        switch(log_lvl)
        {
            case LOGI_TYPE::LOGI_WARN:
                color="\x1b[33m";
                break;
            case LOGI_TYPE::LOGI_ERROR:
                color="\x1b[31m";
                break;
            default:
            break;
        }
        std::string header = color + get_time_for_daytime()+ " %s \x1b[0m";

        char txt_buf[MAX_FILE_LEN+1] = {'\0'};
        
        va_list v_arg;
        va_start (v_arg, format);
        vsnprintf(txt_buf, MAX_FILE_LEN, format, v_arg);
        va_end(v_arg);  

        printf(header.c_str(), txt_buf);             
    }
}

#define LOG(LOG_LVL, fmt, ...) singleton<log_file>::get_instance()->show_log(LOG_LVL, ##__VA_ARGS__);\
                                singleton<log_file>::get_instance()->write_log(LOG_LVL, basename(__FILE__), __LINE__,fmt, ##__VA_ARGS__);

#define LOG_SHOW(LOG_LVL, fmt, ...) singleton<log_file>::get_instance()->show_log(LOG_LVL,fmt, ##__VA_ARGS__);
#define LOG_FILE(LOG_LVL, fmt, ...) singleton<log_file>::get_instance()->write_log(LOG_LVL, basename(__FILE__), __LINE__,fmt, ##__VA_ARGS__);