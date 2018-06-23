#pragma once
#include <memory>

class data_unit
{
public:
    data_unit(std::shared_ptr<char> data = nullptr, unsigned int length = 0)
    :_data(data),
    _length(length)
    {
    }

    data_unit(const data_unit &udata)
    {
        *this = udata;
    }

    data_unit &operator = (const data_unit &udata)
    {
        if(this != &udata)
        {
            this->_data = udata._data;
            this->_length = udata._length;
        }
    }

    std::shared_ptr<char> data() const
    {
        return _data;
    }

    unsigned int length() const
    {
        return _lenth;
    }

private:
    std::shared_ptr<char>  _data;
    unsigned int _length;
};