#pragma once
#include<string>


class IIdGenerator
{
public:
	
    virtual int generateId(std::string, int& id)=0;
    virtual bool removeid(std::string)=0;
    virtual int setStartId(int mid)=0;
    IIdGenerator();
    virtual ~IIdGenerator();
};

