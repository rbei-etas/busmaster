#include "AccessDBManager.h"

AccessDBManager::AccessDBManager()
{
    mParseDbFile = nullptr;
    mDllHandle = nullptr;
    mDbManagerAvailable = false;
    LoadDbManager();
}
bool AccessDBManager::isDbManagerAvailable()
{
    return mDbManagerAvailable;
}
HRESULT AccessDBManager::LoadDbManager()
{
    ReleaseDbmanager();
    mDllHandle = LoadLibrary( "DBManager.dll" );
    if ( nullptr == mDllHandle )
    {
        return S_FALSE;
    }

    mParseDbFile = (PARSEDBFILE)GetProcAddress( mDllHandle, "ParseDBFile" );
    if ( nullptr == mParseDbFile )
    {
        return S_FALSE;
    }

    mFreeCluster = (FREECLUSTER)GetProcAddress( mDllHandle, "FreeCluster" );
    if ( nullptr == mFreeCluster )
    {
        return S_FALSE;
    }
    mDbManagerAvailable = true;
    return S_OK;
}
void AccessDBManager::ReleaseDbmanager()
{
    if ( nullptr == mDllHandle )
    {
        FreeLibrary( mDllHandle );
        mDllHandle = nullptr;
        mParseDbFile = nullptr;
    }
    mDbManagerAvailable = false;
}
AccessDBManager::~AccessDBManager()
{
    ReleaseDbmanager();
}

