#pragma once
#include<string>
#include"Utils_stdafx.h"
#include"IDirectoryUpdate.h"

class IDirectoryWatcher
{
public:
    virtual HRESULT AddDirectoryWatch(std::string strDirName,IDirectoryUpdate* pIDirHdlr)=0;
    virtual HRESULT RemoveDirectoryWatch(std::string strDirName,IDirectoryUpdate* pIDirHdlr)=0;
};