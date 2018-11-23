#pragma once 
#include <unistd.h>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utility.h"
class log_file_io
{
public:
    log_file_io(std::string dir, std::string log_name, std::string type):fd_(-1),dir_(dir),name_(name),type_(type){ symlnk_=log_name+"."+type; }
    ~log_file_io(){ ::close(fd_); fd_=-1; }

    void write(const char *log_txt, int len);

private:
    void ensure_file_size();
    void create_file();
private:
    static int MAX_FILE_SIZE = 1024*1024*1024;
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

inline void log_file_io::write(const char *log_txt, int len)
{
    create_file();
    ensure_file_size();
    ::write(fd_, log_txt, len);
}

inline void log_file_io::create_file()
{
    if(-1 == fd_)
    {
        std::lock_guard<std::mutex> lck(mtx_);
        if(-1 == fd_ )
        {
            path = dir + "/" + name_ + get_time_for_daytime() + type_ + ".txt";
            fd_=::open(path.c_str(), O_RDWR|O_APPEND|O_CREAT|S_IRUSR|S_IWUSR|S_IWGRP|S_IROTH);
            if(-1 != fd_)
            {
                unlink(symlnk_.c_str());
                symlink(path.c_str(), symlnk_.c_str());
            }  
        }
    }
}

inline void log_file_io::ensure_file_size()
{

}