#pragma once
#include<string>
#include"Utils_stdafx.h"

class IDirectoryUpdate
{
public:
    /*DWORD dwAction
    FILE_ACTION_ADDED:              The file was added to the directory.
    FILE_ACTION_REMOVED:            The file was removed from the directory.
    FILE_ACTION_MODIFIED:           The file was modified. This can be a change in the time stamp or attributes.
    FILE_ACTION_RENAMED_OLD_NAME    The file was renamed and this is the old name.
    FILE_ACTION_RENAMED_NEW_NAME    The file was renamed and this is the new name.
    */
    virtual void OnFileChanged(const std::string strFileName,DWORD dwAction)=0;
};