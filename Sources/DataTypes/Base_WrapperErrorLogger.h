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
 * \file      Base_WrapperErrorLogger.h
 * \brief     Base class definition for error logger module
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class definition for error logger module
 */

#pragma once

#define LOG_MESSAGE(WrapperErr, Msg)  {WrapperErr->vLogAMessage(A2T(__FILE__), __LINE__, Msg);}

class Base_WrapperErrorLogger
{
public:
    Base_WrapperErrorLogger(void) {};
    ~Base_WrapperErrorLogger(void) {};

    // Maximum characters in File and Msg are FILENAME_MAX and BUFSIZ respectively
    virtual void vLogAMessage(TCHAR File[], int Line, TCHAR Msg[]) = 0;
};
