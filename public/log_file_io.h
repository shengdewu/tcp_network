#pragma once 
#include <string>
#include <unistd.h>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "utility.h"
class log_file_io
{
public:
    log_file_io(std::string dir, std::string name, std::string type):fd_(-1),dir_(dir),name_(name),type_(type){ symlnk_=dir+"/"+name+"."+type; }
    ~log_file_io(){ ::close(fd_); fd_=-1; }

    void lwrite(const char *log_txt, int len);

private:
    //现在文件大小
    void limit_file_size();
    //创建文件
    void create_file();
private:
    const int MAX_FILE_SIZE = 1024*1024*1024;
    int fd_;
    std::mutex  mtx_;
    std::string dir_;
    std::string name_;
    std::string type_;
    std::string symlnk_;
};

//
//inline
//

inline void log_file_io::lwrite(const char *log_txt, int len)
{
    create_file();
    limit_file_size();
    ::write(fd_, log_txt, len);
}

inline void log_file_io::create_file()
{
    if(-1 == fd_)
    {
        std::lock_guard<std::mutex> lck(mtx_);
        if(-1 == fd_ )
        {
            std::string file_name = name_ + get_time_for_daytime() + "_" + type_ + ".txt";
            std::string path = dir_ + "/" + file_name;
            fd_=::open(path.c_str(), O_RDWR|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR|S_IWGRP|S_IROTH);
            if(-1 == fd_)
            {
                printf("create file %s\n", strerror(errno));
            }  

            unlink(symlnk_.c_str());
            symlink(file_name.c_str(), symlnk_.c_str());
        }
    }
}

inline void log_file_io::limit_file_size()
{
    if(lseek(fd_, 0, SEEK_CUR) > MAX_FILE_SIZE)
    {
        std::unique_lock<std::mutex> lck(mtx_, std::defer_lock);
        if(lck.try_lock())
        {
            std::string path = dir_ + "/" + name_ + get_time_for_daytime() + type_ + ".txt";
            int fd =::open(path.c_str(), O_RDWR|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR|S_IWGRP|S_IROTH);
            if(-1 != fd)
            {
                dup2(fd, fd_);
            }              
        }
    }
}