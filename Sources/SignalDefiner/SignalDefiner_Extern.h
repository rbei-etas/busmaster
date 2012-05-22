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
 * \file      SignalDefiner_Extern.h
 * \brief     Exports API functions for Signal Definer interface  
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2012, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *
 * Exports API functions for Signal Definer interface
 */

#if !defined SIGNAL_DEFINER_EXTERN_H__INCLUDED_
#define SIGNAL_DEFINER_EXTERN_H__INCLUDED_


#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

typedef enum SIGNAL_TYPE
{
	SIGNAL_NONE,
    SINE_WAVE,
    TRIANGULAR_WAVE,  
	COS_WAVE,
}SIGNAL_TYPE;

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

/*  Exported function list */
USAGEMODE HRESULT SignalDefiner_ShowDlg();
USAGEMODE HRESULT SignalDefiner_SetSignalDetails(BSTR strSignalName);
USAGEMODE HRESULT SignalDefiner_GetType(SIGNAL_TYPE* peSignalType);
USAGEMODE HRESULT SignalDefiner_SetType(SIGNAL_TYPE eSignalType);
USAGEMODE HRESULT SignalDefiner_GetAmplitude(float* fAmplitude);
USAGEMODE HRESULT SignalDefiner_SetAmplitude(float fAmplitude);
USAGEMODE HRESULT SignalDefiner_GetSamplingTimePeriod(int* pnSamplingTime);
USAGEMODE HRESULT SignalDefiner_SetSamplingTimePeriod(int nSamplingTime);
USAGEMODE HRESULT SignalDefiner_GetFrequency(float* pfFrequency);
USAGEMODE HRESULT SignalDefiner_SetFrequency(float fFrequency);




#ifdef __cplusplus
}
#endif

#endif // SIGNAL_DEFINER_EXTERN_H__INCLUDED_