#pragma once
#include<string>


class IIdGenerator
{
public:
    int id;
    virtual int generateId(std::string, int& id)=0;
    virtual bool removeid(std::string)=0;
    virtual int setStartId(int mid)=0;
    IIdGenerator();
    ~IIdGenerator();
};

