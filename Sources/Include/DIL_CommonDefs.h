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
 * \authors   Ratnadip Choudhury, Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Some macro definitions necessary
 */

#pragma once

#define LOG_ERR_MSG() sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, A2T((LPSTR) (sg_acErrStr.c_str())))

#define VALIDATE_POINTER_RETURN_VAL(Ptr, RetVal)        if (Ptr == NULL) {return RetVal;}
#define VALIDATE_VALUE_RETURN_VAL(Val1, Val2, RetVal)   if (Val1 != Val2) {return RetVal;}
#define VALIDATE_POINTER_RETURN_VOID(Ptr)               if (Ptr == NULL) {return;}
#define VALIDATE_POINTER_NO_RETURN_LOG(Ptr)             if (Ptr == NULL) {LOG_ERR_MSG();}
#define VALIDATE_POINTER_RETURN_VOID_LOG(Ptr)           if (Ptr == NULL) {LOG_ERR_MSG(); return;}
#define VALIDATE_POINTER_RETURN_VALUE_LOG(Ptr, RetVal)  if (Ptr == NULL) {LOG_ERR_MSG(); return RetVal;}
