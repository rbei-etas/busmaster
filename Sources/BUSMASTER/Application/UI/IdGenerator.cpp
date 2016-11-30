#include "..\stdafx.h"
#include "IdGenerator.h"


int IdGenerator::generateId(std::string menuid, int& id)
{
    std::map<std::string, int>::iterator lastid;
    auto found = menuidlist.find(menuid);

    if (found != menuidlist.end())
    {
        id = found->second;
        return 0;
    }
    else
    {
		id = ++mCurrentId;
		/*if (menuidlist.size() > 1)
		{
			lastid = menuidlist.end();
			--lastid;
			if (id < lastid->second)
			{
				id = lastid->second;
			}
		}*/

		menuidlist.insert(std::pair<std::string, int>(menuid, mCurrentId));
        return 1;
    }
}

bool IdGenerator::removeid(std::string menuid)
{
    auto found = menuidlist.find(menuid);
    if (found != menuidlist.end())
    {
        menuidlist.erase(menuid);
        return true;
    }
    else
    {
        return false;
    }
}

int IdGenerator::setStartId(int mid)
{
	mCurrentId = mid;
	return mCurrentId;
}

