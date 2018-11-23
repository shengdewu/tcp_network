#pragma once
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string>

inline std::string get_time_for_daytime()
{
    timeval tmval;
    gettimeofday(&tmval,nullptr);
    
    struct tm tml;
    localtime_r(&tmval.tv_sec,&tml);
    
	char buffer[16] = { 0 };
	snprintf(buffer,16,"%04d%02d%02d-%02d%02d%02d", tml.tm_year+1900, tml.tm_mon+1, tml.tm_mday,tml.tm_hour, tml.tm_min, tml.tm_sec);

	return std::string(buffer);
}