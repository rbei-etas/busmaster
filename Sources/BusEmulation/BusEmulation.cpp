/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      BusEmulation.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of WinMain
 */

#define STRICT
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit
// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

/* MFC includes */
#include <atlbase.h>
#include <atlcom.h>
#include <time.h>

using namespace ATL;

/* C++ includes */
#include <map>

/* Project includes */
#include "resource_BusSim.h"
#include "BusEmulation.h"

using namespace std;

class CBusEmulationModule : public CAtlExeModuleT< CBusEmulationModule >
{
public :
    DECLARE_LIBID(LIBID_BusEmulationLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BUSEMULATION, "{0937AB35-4C58-4077-9022-B50CB6C42736}")
};

CBusEmulationModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}
