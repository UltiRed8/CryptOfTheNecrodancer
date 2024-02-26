#pragma once

template <typename Class>
class Singleton
{
protected:
    static Class* instance;

public:
    static Class* GetInstance()
    {
        if (!instance) instance = new Class();
        return instance;
    }

public:
    Singleton() {}
    ~Singleton()
    {
        delete instance;
    }
};

template <typename Class>
Class* Singleton<Class>::instance = nullptr;