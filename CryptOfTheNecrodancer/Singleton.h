#pragma once

template <typename Class>
class Singleton
{
protected:
    static Class instance;

public:
    static Class& GetInstance()
    {
        return instance;
    }

public:
    Singleton() {}
};

template <typename Class>
Class Singleton<Class>::instance;