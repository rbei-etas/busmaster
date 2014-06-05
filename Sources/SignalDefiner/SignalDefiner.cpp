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
 * \file      SignalDefiner.cpp
 * \brief     Defines the initialization routines for the DLL.
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *
 * Defines the initialization routines for the DLL.
 */

#include "SignalDefiner_StdAfx.h"
#include "SignalDefiner.h"
#include "SignalDefinerDlg.h"
#include "../Application/MultiLanguage.h"

#define USAGE_EXPORT
#include "SignalDefiner_Extern.h"

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//      any functions exported from this DLL which call into
//      MFC must have the AFX_MANAGE_STATE macro added at the
//      very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//


// CSignalDefinerApp

BEGIN_MESSAGE_MAP(CSignalDefinerApp, CWinApp)
END_MESSAGE_MAP()


// CSignalDefinerApp construction

CSignalDefinerApp::CSignalDefinerApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CSignalDefinerApp object

CSignalDefinerApp theApp;


// CSignalDefinerApp initialization
static HINSTANCE ghLangInst=nullptr;

BOOL CSignalDefinerApp::InitInstance()
{
    // Begin of Multiple Language support
    if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(false);
    }
    // Load resource-only language DLL. It will use the languages
    // detected above, take first available language,
    // or you can specify another language as second parameter to
    // LoadLangResourceDLL. And try that first.
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }
    // End of Multiple Language support

    CWinApp::InitInstance();

    return TRUE;
}

int CSignalDefinerApp::ExitInstance()
{
    int result = CWinApp::ExitInstance();
    if (0 == result)
    {
        // This DLL is natively linked against the "new" runtime
        // library, and thus uses that library's allocators
        // for "new" calls during DllMain.

        // When MFC cleans up during AfxTermExtensionModule,
        // the de-allocator being called is the one from the "old"
        // runtime library (against which the MFC DLL itself is
        // linked). That's okay, but it should not attempt to
        // destroy any object created with the new allocator.

        // Unfortunately, there's one single CDynLinkLibrary
        // object being created in our DllMain -- with the new
        // allocator.

        // Fortunately, that object is expected to be the head
        // in the list of active libraries.

        AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
        ASSERT(pModuleState);

        CDynLinkLibrary* pDLL = pModuleState->m_libraryList;
        if (pDLL &&
                (pDLL->m_hModule == pModuleState->m_hCurrentInstanceHandle))
        {
            // We got it! Remove it, ...
            pModuleState->m_libraryList.Remove(pDLL);
            // ... run the destructor, ...
            pDLL->~CDynLinkLibrary();
            // ... and free the memory -- with our native
            // deallocator!
            //free(pDLL);
        }
    }
    return result;
}

/*Singal Definer variable declaration*/
CSignalDefinerDlg g_objSignalDefiner;

/**
* \brief         Shows the signal definer dialog
* \param[out]    -
* \param[in]     -
* \return        S_OK on success and if USER presses OK button, S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_ShowDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return g_objSignalDefiner.DoModal();
}

/**
* \brief         Gets the Signal details to display in Signal definer dialog
* \param[out]    -
* \param[in]     Signal Name as 'CString'
* \return        S_OK on success and if USER presses OK button, S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_SetSignalDetails(BSTR strSignalName)
{
    g_objSignalDefiner.m_strSignalName = strSignalName;
    return S_OK;
}

/**
* \brief         Setter for signal type
* \param[out]    -
* \param[in]     Gets the signal type in enum 'eSignalType'
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_SetType(SIGNAL_TYPE eSignalType)
{
    g_objSignalDefiner.m_nSignalType = eSignalType;

    return S_OK;
}

/**
* \brief         Getter for signal type
* \param[out]    Gives back the signal type in enum 'peSignalType'
* \param[in]     -
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_GetType(SIGNAL_TYPE* peSignalType)
{
    *peSignalType = (SIGNAL_TYPE)g_objSignalDefiner.m_nSignalType;

    return S_OK;
}

/**
* \brief         Setter for signal amplitude
* \param[out]    -
* \param[in]     Gets the signal amplitude in float 'fAmplitude'
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_SetAmplitude(float fAmplitude)
{
    g_objSignalDefiner.m_fAmplitude = fAmplitude;

    return S_OK;
}

/**
* \brief         Getter for signal amplitude
* \param[out]    Gives back the signal amplitude in float 'pfAmplitude'
* \param[in]     -
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_GetAmplitude(float* pfAmplitude)
{
    *pfAmplitude = g_objSignalDefiner.m_fAmplitude;

    return S_OK;
}

/**
* \brief         Setter for signal Sampling TimePeriod
* \param[out]    -
* \param[in]     Gets the signal Sampling TimePeriod in int 'nSamplingTime'
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_SetSamplingTimePeriod(int nSamplingTime)
{
    g_objSignalDefiner.m_dblSamplingTimePeriod = nSamplingTime;
    return S_OK;
}

USAGEMODE HRESULT SignalDefiner_GetAutoCorrect(bool* pbChecked)
{
    *pbChecked = g_objSignalDefiner.m_bAutoCorrect;
    return S_OK;
}
USAGEMODE HRESULT SignalDefiner_SetAutoCorrect(bool bChecked)
{
    g_objSignalDefiner.m_bAutoCorrect = bChecked;
    return S_OK;
}
/**
* \brief         Getter for signal Sampling TimePeriod
* \param[out]    Gives back the signal Sampling TimePeriod in int 'pnSamplingTime'
* \param[in]     -
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_GetSamplingTimePeriod(int* pnSamplingTime)
{

    *pnSamplingTime = (int)g_objSignalDefiner.m_dblSamplingTimePeriod;

    return S_OK;
}

/**
* \brief         Setter for signal Frequency
* \param[out]    -
* \param[in]     Gets the signal Frequency in double 'dblFrequency'
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_SetFrequency(float dblFrequency)
{
    g_objSignalDefiner.m_fFrequency = dblFrequency;

    return S_OK;
}

/**
* \brief         Getter for signal Frequency
* \param[out]    Gives back the signal Frequency in double 'pdblFrequency'
* \param[in]     -
* \return        S_OK on success , S_FALSE otherwise
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
USAGEMODE HRESULT SignalDefiner_GetFrequency(float* pdblFrequency)
{
    *pdblFrequency = g_objSignalDefiner.m_fFrequency;

    return S_OK;
}

