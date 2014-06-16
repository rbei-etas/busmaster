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
 * @brief Interface file for CUtilFunctions class
 * @author Raja N, Tobias Lorenz
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CUtilFunctions class
 */

#pragma once

#include <iostream>

class CUtilFunctions
{
public:
    /**
     * @brief Top mask unwanted bit of the data
     *
     * This function will set the unsued portion to zero. This is
     * required to get the 2's value of a signal lessage then 64
     * bit length
     *
     * @param[inout] n64rData data
     * @param[in] nLength Valid Data length
     */
    static void s_vRemoveUnwantedBits(__int64& n64rData, int nLength);

    /**
     * @brief To extend the sign bit of a signal to get 64 bit euavelent value
     *
     * This function extends the sign bit of a data to get actual
     * value of a signed number that is stored in 2s complement form
     *
     * @param[inout] n64Val data
     * @param[in] nSize Valid Data length
     */
    static void s_vExtendSignBit( __int64& n64Val, int nSize);

    static void MakeRelativePath(const char* currentDirectory, char* absoluteFilename, std::string& omStrRelativeParh);

    static INT nGetBaseFolder(const char* omConfigFileName, std::string& omStrConfigFolder);

private:
    /* Make constructor as private so that object can not be created for this class */
    CUtilFunctions();
    virtual ~CUtilFunctions();
};
