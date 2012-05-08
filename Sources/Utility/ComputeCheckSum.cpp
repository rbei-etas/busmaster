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
 * \file      ComputeCheckSum.cpp
 * \brief     This file contains implementation of CComputeCheckSum class
 * \author    Anish Kumar, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains implementation of CComputeCheckSum class
 */

#include "Utils_stdafx.h"
#include "ComputeCheckSum.h"    // For CComputeCheckSum class definition

#define defSIZE_OF_ERROR_BUFFER 1024
#define defPROJECT_NAME         _T("MCNet")

/**
 * \brief Constructor
 *
 * Standard default constructor
 */
CComputeCheckSum::CComputeCheckSum()
{
}

/**
 * \brief Destructor
 *
 * Standard Destructor
 */
CComputeCheckSum::~CComputeCheckSum()
{
}

/**
 * \brief      Compute Check Sum
 * \param[in]  pucBuff pointer to buffer
 * \param[in]  dwSize number of bytes for which checksum to be computed
 * \param[out] pucCheckSum pointer to computed checksum value byte
 * \return     It return TRUE on success.
 *
 * This method computes the checksum for a give bytes
 * of data passed as pointer pucBuff.
 */
BOOL CComputeCheckSum::bComputeCheckSum(UCHAR* pucBuff,
                                        DWORD dwSize,
                                        UCHAR* pucCheckSum)
{
    BOOL  bReturn     = FALSE;

    // check for pointer and size to be more then zero.
    if(pucBuff != NULL && dwSize >0)
    {
        UCHAR ucTempByte  = 0;

        for(UINT unIndex = 0; unIndex < dwSize; unIndex++)
        {
            // XOR the bytes with result and update the result. First time
            // the result will be zero and XOR will return the byte itself.
            ucTempByte =  static_cast<UCHAR>(ucTempByte^ pucBuff[unIndex]);
        }

        // check for the pointer to be NULL. If not NULL  assign the
        // computed checksum
        if(pucCheckSum != NULL)
        {
            *pucCheckSum = ucTempByte;
            bReturn = TRUE;
        }
    }

    return bReturn;
}

/**
 * \brief      Get Check Sum
 * \param[in]  omStrConfigFileName reference to File name
 * \param[out] pucCheckSum Returns computed check sum
 * \param[out] pucCheckSumInFile : Returns the chechsum in file
 * \return     TRUE for success and FALSE for failure
 *
 * This method computes the checksum for a given bytes
 * in a file. The file is opened and checksum is computed
 * for all bytes except the last byte.
 */
BOOL CComputeCheckSum::bGetCheckSum(CString& omStrConfigFileName,
                                    UCHAR* pucCheckSum, UCHAR* pucCheckSumInFile)
{
    CStdioFile omStdiofile;
    CFileException   omException  ;
    DWORD dwSize                 = 0;
    DWORD dwRead                 = 0;
    BOOL bReturn                 = FALSE;
    BOOL bFileOpen               = FALSE;
    CString omStrErrorMessage    = "";
    char  acErrorMsg[defSIZE_OF_ERROR_BUFFER];
    // Open the configration file.
    TRY
    {
        bFileOpen = omStdiofile.Open(omStrConfigFileName,
        CFile::modeRead | CFile::typeBinary, &omException);

        if(bFileOpen!=FALSE)
        {
            UCHAR* pucBuff   = NULL;
            // Get the size of file
            dwSize = (DWORD)omStdiofile.GetLength();

            if( dwSize > 0)
            {
                pucBuff = static_cast<UCHAR*> (new UCHAR[dwSize]);

                if(pucBuff!=NULL)
                {
                    // Read the whole file and put the content to pucBuff;
                    dwRead = omStdiofile.Read(pucBuff,dwSize);
                    // Compute the checksum
                    bReturn = bComputeCheckSum(pucBuff,dwSize - 1,
                    pucCheckSum);
                    // Get the check sum stored in file ( Last byte )
                    *pucCheckSumInFile  = pucBuff[dwSize-1];
                    delete [] pucBuff;
                    pucBuff = NULL;
                }
            }

            omStdiofile.Close();
            bReturn = TRUE;
        }
        else
        {

            // Get the exception error message
            omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            // configuration file  open error notification
            //AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,nZERO);
            MessageBox(NULL,acErrorMsg,
            defPROJECT_NAME,MB_OK|MB_ICONERROR|MB_TOPMOST);

        }

    }
    CATCH_ALL(pomE)
    {
        if(pomE != NULL )
        {
            // Get the exception error message
            pomE->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            //Display the error
            //AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,nZERO);
            MessageBox(NULL,acErrorMsg,
                       defPROJECT_NAME,MB_OK|MB_ICONERROR|MB_TOPMOST);
        }
    }
    END_CATCH_ALL
    return bReturn;
}

/**
 * \brief      Set Check Sum
 * \param[in]  omStrConfigFileName reference to File name
 * \param[out] pucCheckSum Returns computed check sum
 * \return     Returns TRUE for sucess and FALSE for failure.
 *
 * This method computes the checksum for a given bytes
 * in a file. The file is opened and checksum is computed
 * for all bytes except the last byte. The same checksum is
 * overwritten at the last byte in fileSuccess.
 */
BOOL CComputeCheckSum::bSetCheckSum(CString& omStrConfigFileName,
                                    UCHAR* pucCheckSum)
{
    CStdioFile omStdiofile;
    CFileException omException ;
    DWORD dwSize                 = 0;
    DWORD dwRead                 = 0;
    BOOL bReturn                 = FALSE;
    BOOL bFileOpen               = FALSE;
    CString omStrStrErrorMessage = "";
    char  acErrorMsg[defSIZE_OF_ERROR_BUFFER];
    // Open the configration file template.
    TRY
    {
        bFileOpen = omStdiofile.Open(omStrConfigFileName,
        CFile::modeReadWrite| CFile::typeBinary,
        &omException);

        if(bFileOpen!=FALSE)
        {
            UCHAR* pucBuff   = NULL;
            UCHAR  ucCheckSum = 0;
            // Get the size of file
            dwSize = (DWORD)omStdiofile.GetLength();
            pucBuff = static_cast<UCHAR*> (new UCHAR[dwSize]);

            if(pucBuff!=NULL)
            {
                // Read the whole file and put the content to pucBuff;
                dwRead = omStdiofile.Read(pucBuff,dwSize);
                // compute the checksum
                bReturn = bComputeCheckSum(pucBuff,dwSize, &ucCheckSum);

                if(bReturn == TRUE)
                {
                    // Seek to the last byte to store checksum
                    omStdiofile.Seek(dwSize,CFile::begin);
                    // Write one byte checksum
                    omStdiofile.Write(&ucCheckSum,1);

                    // return the checksum
                    if(pucCheckSum!= NULL )
                    {
                        *pucCheckSum = ucCheckSum;
                    }
                    else
                    {
                        bReturn = FALSE;
                    }
                }

                delete [] pucBuff;
                pucBuff = NULL;
            }

            omStdiofile.Close();
        }
        else
        {
            // Get the exception error message
            omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            // configuration file open error notification
            MessageBox(NULL, acErrorMsg,
            defPROJECT_NAME, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
            //AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);

        }

    }
    CATCH_ALL(pomE)
    {
        if(pomE != NULL )
        {
            // Get the exception error message
            pomE->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            //Display the error
            MessageBox(NULL, acErrorMsg,
                       defPROJECT_NAME, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
            //AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
            pomE->Delete();
        }
    }
    END_CATCH_ALL
    return bReturn;
}

/**
 * \brief      Set Check Sum
 * \param[in]  omStrConfigFileName reference to File name
 * \param[out] pucCheckSum Returns computed check sum
 * \return     Returns TRUE for sucess and FALSE for failure
 *
 * This method computes the checksum for a given bytes
 * in a file. The file is opened and checksum is computed
 * for all bytes except the last byte. The same checksum is
 * overwritten at the last byte in file. For COM Support
 */
BOOL CComputeCheckSum::COM_bSetCheckSum(CString& omStrConfigFileName,
                                        UCHAR* pucCheckSum, CString& strError)
{
    CStdioFile omStdiofile;
    CFileException omException;
    DWORD dwSize                 = 0;
    DWORD dwRead                 = 0;
    BOOL bReturn                 = FALSE;
    BOOL bFileOpen               = FALSE;
    CString omStrStrErrorMessage = "";
    char  acErrorMsg[defSIZE_OF_ERROR_BUFFER];
    // Open the configration file template.
    TRY
    {
        bFileOpen = omStdiofile.Open(omStrConfigFileName,
        CFile::modeReadWrite| CFile::typeBinary,
        &omException);

        if(bFileOpen!=FALSE)
        {
            UCHAR* pucBuff   = NULL;
            UCHAR  ucCheckSum = 0;
            // Get the size of file
            dwSize = (DWORD)omStdiofile.GetLength();
            pucBuff = static_cast<UCHAR*> (new UCHAR[dwSize]);

            if(pucBuff!=NULL)
            {
                // Read the whole file and put the content to pucBuff;
                dwRead = omStdiofile.Read(pucBuff,dwSize);
                // compute the checksum
                bReturn = bComputeCheckSum(pucBuff,dwSize, &ucCheckSum);

                if(bReturn == TRUE)
                {
                    // Seek to the last byte to store checksum
                    omStdiofile.Seek(dwSize,CFile::begin);
                    // Write one byte checksum
                    omStdiofile.Write(&ucCheckSum,1);

                    // return the checksum
                    if(pucCheckSum!= NULL )
                    {
                        *pucCheckSum = ucCheckSum;
                    }
                    else
                    {
                        bReturn = FALSE;
                    }
                }

                delete [] pucBuff;
                pucBuff = NULL;
            }

            omStdiofile.Close();
        }
        else
        {
            // Get the exception error message
            omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            // configuration file open error notification
        }

    }
    CATCH_ALL(pomE)
    {
        if(pomE != NULL )
        {
            // Get the exception error message
            pomE->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
        }
    }
    END_CATCH_ALL
    strError = acErrorMsg;
    return bReturn;
}
