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

/**********************************************************************************************************************
 *
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_Log.cpp
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Implements function to log information.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */


/* project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_Log.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_extern.h"

/* includes */
#include <string>
#include <fstream>
#include <sstream>


/**
 * This function takes the passed string and logs it.
 */
void flexi_Log_String(std::string strLogFile, const char* logging_string)
{

    /* lock to prevent concurrent access to Logfile */
    g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->lockLogfile.lock();

    /* write string in corresponding file */
    std::ofstream f;
    f.open(strLogFile.c_str(), std::ios::app);

    f << logging_string << std::endl;

    f.close();


    /* release lock */
    g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->lockLogfile.unlock();
}

