#pragma once
#include "log_file_io.h"

enum LOGI_TYPE
{
    LOGI_DEBUG=0,
    LOGI_WARN,
    LOGI_ERROR,
};

class log_file
{
public:
    log_file():debug_("log", get_process_name(), "debug"),warn_("log", get_process_name(), "warn"),error_("log", get_process_name(), "error"){}

private:
    log_file_io debug_;
    log_file_io warn_;
    log_file_io error_;
};