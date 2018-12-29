#pragma once

template<class T>
class singleton
{
public:
    static T* get_instance();
};

//
//
//

template<class T>
T* singleton<T>::get_instance()
{
    static T instance;
    return &instance;
}