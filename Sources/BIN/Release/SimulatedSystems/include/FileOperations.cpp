#include "CANIncludes.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

char pchWriteDirectory[MAX_PATH] = {'\0'};
char pchReadDirectory[MAX_PATH] = {'\0'};
int nWritePathChanged = 1;
int nReadPathChanged = 1;
extern "C" IMAGE_DOS_HEADER __ImageBase;
void InitialiseDllPath()
{
    char pchDllPath[MAX_PATH];
    static const basic_string <char>::size_type npos = -1;
    GetModuleFileName(((HINSTANCE)&__ImageBase), pchDllPath, MAX_PATH);
    string strPath = pchDllPath;
    INT nIndex = strPath.find_last_of('\\');
    if(nIndex != npos)
    {
        strPath.copy(pchWriteDirectory, nIndex+1);
        strPath.copy(pchReadDirectory, nIndex+1);
    }
}



/* CAPL Function Details                                                */
/* Syntax:      long fileClose (UINT fileHandle);                       */
/* Parameters:  The integer contains the handle to the file.            */
/* Return Value: If an error occurs the return value is 0, else 1.      */

long fileclose_CAPL (UINT dwFileHandle)
{
    int nResult;
    FILE* pFile = (FILE*)dwFileHandle;
    //fclose returns 0 if the stream is successfully closed
    nResult = fclose (pFile);
    return ((nResult == 0)? 1 : 0);
}

/* CAPL Function Details                                                    */
/* Syntax:      UINT openFileRead (char filename[], UINT mode);         */
/* Parameters:  file name and mode                                          */
/*                  If mode=0 the file is opened in ASCII mode;             */
/*                  if mode=1 the file is opened in binary mode             */
/* Return Value: The return value is the file handle that                   */
/*                       must be used for read operations.                  */
/*               If an error occurs the return value is 0.                  */

UINT openfileread_CAPL (char pchFileName[], UINT dwMode)
{
    Trace("In File open %s", pchFileName);
    FILE* pFile = NULL;
    char pchFileMode[3] = {'\0'};
    if(dwMode == 0)
    {
        pFile = fopen(pchFileName, "r");
        pFile = fopen(pchFileName, "r");
        strcpy(pchFileMode, "r");
    }
    else if(dwMode == 1)
    {
        pFile = fopen(pchFileName, "rb");
        strcpy(pchFileMode, "rb");
    }
    if(pFile == NULL)
    {
        pFile = fopen(pchFileName, "w");
        if(pFile != NULL)
        {
            fclose(pFile);
            fopen(pchFileName, pchFileMode);
        }
    }
    return ((UINT)(pFile));
}


/* CAPL Function Details                                                    */
/* Syntax:      UINT openFileWrite (char filename[], UINT mode);        */
/* Parameters:  file name and mode                                          */
/*                  If mode=0 writing can be executed in ASCII mode;        */
/*                      An already existing file will be overwritten.       */
/*                  if mode=1 writing can be executed in binary mode.       */
/*                      An already existing file will be overwritten.       */
/*                  If mode=2 to append data at the end of the file         */
/*                      use for ASCII mode.                                 */
/*                  If mode=3 to append data at the                         */
/*                      end of the file for binary mode.                    */
/* Return Value: The return value is the file handle that must be           */
/*                  used for write operations.                              */
/*               If an error occurs the return value is 0.                  */

UINT openfilewrite_CAPL (char pchFileName[], UINT dwMode)
{
    FILE* pFile = NULL;

    if(nWritePathChanged != 0)
    {
        InitialiseDllPath();
        SetCurrentDirectory(pchWriteDirectory);
        nWritePathChanged = 0;
    }

    switch(dwMode )
    {
        case 0:                                 //ASCII-OVERWRITE
            pFile = fopen(pchFileName, "w");
            break;

        case 1:                                 //BINARY-OVERWRITE
            pFile = fopen(pchFileName, "wb");
            break;

        case 2:                                 //ASCII-APPEND
            pFile = fopen(pchFileName, "a");
            break;

        case 3:                                 //ASCII-OVERWRITE
            pFile = fopen(pchFileName, "ab");
            break;

        default:
            pFile = NULL;
            break;
    }
    return ((UINT)(pFile));
}

/* CAPL Function Details                                                    */
/* Syntax:      long fileGetBinaryBlock (byte buff[], long buffsize,        */
/*                                                  UINT fileHandle)        */
/* Parameters:  buff is the output  Buffer                                  */
/*              buffsize is the Maximum of buffsize characters              */
/*              fileHandle is the Handle to the file                        */
/* Return Value: The function returns the number of characters read.        */
long filegetbinaryblock_CAPL (byte byBuffer[], long lBufferSize, UINT dwFileHandle)
{
    if( NULL != (void*)dwFileHandle )
    {
        return fread((void*)byBuffer, 1, lBufferSize, (FILE*)dwFileHandle);
    }
    else
    {
        return 0;
    }
}

/* CAPL Function Details                                                    */
/* Syntax:      long fileGetStringSZ(char buff[], long buffsize,            */
/*                                              UINT fileHandle)            */
/* Parameters:  buff is the output  Buffer                                  */
/*              buffsize is the Maximum of buffsize characters              */
/*              fileHandle is the Handle to the file                        */
/* Return Value: If an error occurs the return value is 0, else 1.          */
long filegetstring_CAPL (char pchBuffer[], long lBuffSize, UINT dwFileHandle)
{
    int nRetVal = 0;
    if( NULL != (void*)dwFileHandle )
    {
        nRetVal = 1;
        if( fgets (pchBuffer, lBuffSize, (FILE*)dwFileHandle) == NULL)
        {
            nRetVal = 0;
        }
    }
    return nRetVal;
}


/* CAPL Function Details                                                    */
/* Syntax:      long filePutString (char buff[], long buffsize,             */
/*                                              UINT fileHandle);           */
/* Parameters:  buff is the output  Buffer                                  */
/*              buffsize is the Maximum of buffsize characters              */
/*              fileHandle is the Handle to the file                        */
/* Return Value: If an error occurs the return value is 0, else 1.          */
long fileputstring_CAPL (char pchBuffer[], long lBuffSize, UINT dwFileHandle)
{
    //TODO::
    int nRetVal = 1;
    int nStringLength;
    char* pchTempString;
    lBuffSize += 1;
    pchTempString = (char*)malloc(lBuffSize*sizeof(char));
    memset(pchTempString, 0, lBuffSize);
    strncpy(pchTempString, pchBuffer, lBuffSize);
    pchTempString[lBuffSize-1] = '\0';
    fputs( pchTempString, (FILE*)dwFileHandle );
    free ( pchTempString);

    return nRetVal;
}

/* CAPL Function Details                                                    */
/* Syntax:      long fileRewind (UINT fileHandle)                           */
/* Parameters:  fileHandle is the Handle to the file                        */
/* Return Value: If an error occurs the return value is 0, else 1.          */
long filerewind_CAPL (UINT dwFileHandle)
{
    int nRetVal;
    if( NULL != (void*)dwFileHandle )
    {
        nRetVal = fseek((FILE*)dwFileHandle, 0, SEEK_SET);
    }
    return ((nRetVal > 0)? 1 : 0);
}

/* CAPL Function Details                                                    */
/* Syntax:      long fileWriteBinaryBlock (byte buff[], long buffsize,      */
/*                                                  UINT fileHandle)        */
/* Parameters:  buff is the input   Buffer                                  */
/*              buffsize is the Maximum of buffsize characters              */
/*              fileHandle is the Handle to the file                        */
/* Return Value: The function returns the number of characters read.        */
long filewritebinaryblock_CAPL (byte byBuffer[], long lBufferSize, UINT dwFileHandle)
{
    if( NULL != (void*)dwFileHandle )
    {
        return fwrite((void*)byBuffer, 1, lBufferSize, (FILE*)dwFileHandle);
    }
    else
    {
        return 0;
    }
}
/* CAPL Function Details                                                    */
/* Syntax:      long getProfileInt(char section[], char entry[],            */
/*                                          long def, char filename[])      */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              def is Default value in case of error as an integer.        */
/*              filename is File path as a string.                          */
/* Return Value: Integer that was read in.                                  */
long getprofileint_CAPL (char pchSection[], char pchEntry[], long lDefVal,
                         char pchFileName[])
{
    return GetPrivateProfileInt(pchSection, pchEntry, lDefVal, pchFileName);
}


/* CAPL Function Details                                                    */
/* Syntax:      long getProfileString(char section[], char entry[],         */
/*                          char def[], char buff[], long buffsize,         */
/*                          char filename[])                                */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              buff is Buffer for the read-in character as a string.       */
/*              def is Default value in case of error as an integer.        */
/*              buffersize is Size of buff in bytes.                        */
/*              filename is File path as a string.                          */
/* Return Value: Number of characters that were read.                       */

long getprofilestring_CAPL (char pchSection[], char pchEntry[], char pchDefVal[],
                            char pchReturnBuff[],   long lBuffSize, char pchFileName[] )
{
    return (long)GetPrivateProfileString(pchSection, pchEntry, pchDefVal,
                                         pchReturnBuff, lBuffSize ,pchFileName);
}


/* CAPL Function Details                                                    */
/* Syntax:      long writeProfileInt(char section[], char entry[],          */
/*                                          long value, char filename[])    */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              value as a integer to write.                                */
/*              filename is File path as a string.                          */
/* Return Value: The functional result is 0 in case of an error.            */
long writeprofileint_CAPL (char pchSection[], char pchEntry[], long lVal,
                           char pchFileName[])
{
    char pchStringVal[32];
    if(nWritePathChanged != 0)
    {
        InitialiseDllPath();
        SetCurrentDirectory(pchWriteDirectory);
        nWritePathChanged = 0;
    }
    sprintf(pchStringVal, "%d", lVal);
    return WritePrivateProfileString(pchSection, pchEntry, pchStringVal,
                                     pchFileName);
}

/* CAPL Function Details                                                        */
/* Syntax:      long long writeProfileString(char section[], char entry[],
                                char value[], char filename[]);             */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              value is Value as a string.                         .       */
/*              filename is File path as a string.                          */
/* Return Value: Number of characters that were read.                       */

long writeprofilestring_CAPL (char pchSection[], char pchEntry[], char pchVal[],
                              char pchFileName[] )
{
    //TODO::RetValue
    if(nWritePathChanged != 0)
    {
        InitialiseDllPath();
        SetCurrentDirectory(pchWriteDirectory);
        nWritePathChanged = 0;
    }
    return (long)WritePrivateProfileString(pchSection, pchEntry, pchVal,
                                           pchFileName);
}

/* CAPL Function Details                                                    */
/* Syntax:      long writeProfileFloat(char section[], char entry[],        */
/*                                          float value, char filename[])   */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              value as a integer to write.                                */
/*              filename is File path as a string.                          */
/* Return Value: The functional result is 0 in case of an error.            */
long writeprofilefloat_CAPL (char pchSection[], char pchEntry[], float fVal,
                             char pchFileName[])
{
    char pchStringVal[256];
    if(nWritePathChanged != 0)
    {
        InitialiseDllPath();
        SetCurrentDirectory(pchWriteDirectory);
        nWritePathChanged = 0;
    }
    sprintf(pchStringVal, "%f", fVal);
    return WritePrivateProfileString(pchSection, pchEntry, pchStringVal,
                                     pchFileName);
}


/* CAPL Function Details                                                    */
/* Syntax:      long getProfileFloat(char section[], char entry[],          */
/*                                          long def, char filename[])      */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              Default value in case of error as a float.                  */
/*              filename is File path as a string.                          */
/* Return Value: The functional result is 0 in case of an error.            */
float getprofilefloat_CAPL ( char pchSection[], char pchEntry[], long lDefVal,
                             char pchFileName[] )
{
    char pchTempString[256] = {0};
    float fValue;
    GetPrivateProfileString( pchSection, pchEntry, "NULL", pchTempString,
                             sizeof(pchTempString), pchFileName );

    if( !strcmp( pchTempString, "NULL" ) || !stricmp( pchTempString, "" ) )
    {
        return (float)lDefVal;
    }

    fValue = atof( pchTempString );
    return fValue;
}

/* CAPL Function Details                                                    */
/* Syntax:      long getProfileArray(char section[], char entry[],          */
/*                                  char buff[], long buffsize,             */
/*                                  char filename[]                         */
/* Parameters:  section is Section of the file as a string.                 */
/*              entry is Variable name as a string.                         */
/*              buff Buffer for the read-in characters.                     */
/*              buffsize Size of buff (max. 1279 characters).               */
/*              filename is File path as a string.                          */
/* Return Value: Number of characters read in.                              */
long getprofilearray_CAPL (char pchSection[], char pchEntry[],char pchBuff[],
                           long lBuffSize, char pchFileName[])
{
    GetPrivateProfileStruct(pchSection, pchEntry, pchBuff,
                            lBuffSize, pchFileName);
    return strlen(pchBuff);
}
/* CAPL Function Details                                                    */
/* Syntax:      void setWritePath ();                                       */
/* Parameters:  relativeOrAbsolutePath is the Dircetory path                */
/* Return Value: Number of characters read in.                              */
void setwritepath_CAPL(char pchRelOrAbsPath[])
{
    strncpy(pchWriteDirectory, pchRelOrAbsPath, MAX_PATH);
    nWritePathChanged = 1;
}

/* CAPL Function Details                                                    */
/* Syntax:      void setFilePath (char Path[],UINT mode);                   */
/* Parameters:  path is the Dircetory path                                  */
/*              0 Sets path for read functions                              */
/*              1 Sets path for write functions (same as )                  */
/*              2 Sets path for both types of functions                     */
/* Return Value: Number of characters read in.                              */

void setfilepath_CAPL(char pchDirPath[], UINT dwMode)
{
    switch(dwMode)
    {
        case 0:
            strncpy(pchReadDirectory, pchDirPath, MAX_PATH);
            nReadPathChanged = 1;
            break;
        case 1:

            break;
        case 2:
            strncpy(pchReadDirectory, pchDirPath, MAX_PATH);
            setwritepath_CAPL(pchDirPath);
            nReadPathChanged = 1;
            break;
        default:
            break;
    }
}







