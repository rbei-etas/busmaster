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
 * \file      DIL_CommonDefs.h
 * \brief     Some macro definitions necessary
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Some macro definitions necessary
 */

#pragma once

/* Common DIL macros starts*/
#include "DataTypes/DIL_Datatypes.h"

static bool bIs_NULL_Pointer(char* /*PtrName*/, void* Ptr)
{
    return (Ptr == NULL);
}

#define LOG_ERR_MSG() sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, A2T((LPSTR) (sg_acErrStr.c_str())))

#define VALIDATE_POINTER_RETURN_VAL(Ptr, RetVal) if (bIs_NULL_Pointer(#Ptr, Ptr)) {return RetVal;}
#define VALIDATE_VALUE_RETURN_VAL(Val1, Val2, RetVal)   if (Val1 != Val2) {return RetVal;}
#define VALIDATE_POINTER_RETURN_VOID(Ptr)               if (bIs_NULL_Pointer(#Ptr, Ptr)) {return;}
#define VALIDATE_POINTER_NO_RETURN_LOG(Ptr)             if (bIs_NULL_Pointer(#Ptr, Ptr)) {LOG_ERR_MSG();}
#define VALIDATE_POINTER_RETURN_VOID_LOG(Ptr)           if (bIs_NULL_Pointer(#Ptr, Ptr)) {LOG_ERR_MSG(); return;}
#define VALIDATE_POINTER_RETURN_VALUE_LOG(Ptr, RetVal)  if (bIs_NULL_Pointer(#Ptr, Ptr)) {LOG_ERR_MSG(); return RetVal;}

//#define INITIALISE_DATA(Data)   memset(&Data, 0, sizeof(Data))
//#define INITIALISE_ARRAY(Array) memset(Array, 0, sizeof(Data))
#define MAX_HW_INTERFACE 4
/* Common DIL macros ends*/
