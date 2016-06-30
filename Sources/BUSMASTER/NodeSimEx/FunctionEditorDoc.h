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
 * \file      FunctionEditorDoc.h
 * \brief     This header file contains the defintion of class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "NodeSimEx_Struct.h"
#include "FunctionEditorStruct.h"
#include "DataTypes\MsgSignal_Datatypes.h"
#include <list>
#include <regex>
class CGlobalObj;

class CFunctionEditorDoc : public CDocument
{
private:
    bool m_bNotifyReload;
    CGlobalObj* m_pGloabalObj;

protected: // create from serialization only
    bool m_bFileSavedFromBM;
    SBUS_SPECIFIC_INFO m_sBusSpecInfo;
    CStringArray m_omGlobalVariableArray;

    void Initialize();
    virtual void vInitializeBusSpecificHandlers();

    CFunctionEditorDoc(CGlobalObj* pGlobalObj = nullptr);
    DECLARE_DYNCREATE(CFunctionEditorDoc)

    // Attributes
public:
    CStringList m_omSourceCodeTextList;
    CStringArray m_omFunctionNameArray;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFunctionEditorDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName, CGlobalObj* pGlobalObj = nullptr, bool bIsOldFormat = false);
    virtual void OnCloseDocument();
    virtual CString omGetMsgHandlerName();
    //}}AFX_VIRTUAL

    // Implementation
public:

    // Function to get initialised string for database messages
    CString omStrGetInitialisedMessage( UINT unMsgID,
                                        const CString& omStrMsgName,
                                        const CString& omStrVarName,
                                        BOOL bInitData,
                                        int nChannelId, INT unChannelSel = 1);
    CStringArray* pomStrGetErrorHandlerPrototypes();
    CStringArray* pomStrGetDLLHandlerPrototypes();
    CStringArray* pomStrGetBusEventHandlerPrototypes();
    CStringArray* omStrGetEventIndPrototypes();
    CStringArray* omStrGetIncudeHeaderFiles();
    int nGetMaxLineLength();
    CString omStrExtractFunctionNameFromPrototype ( CString omStrPrototype);
    CStringArray* omStrGetFunctionNamesForDEF();
    CStringArray* omStrGetGlobalVariablePrototypes();
    CStringArray* omStrGetUtilityFunctionPrototypes();
    CStringArray* omStrGetKeyHandlerPrototypes();
    /* CStringArray* omStrGetCycleHandlerPrototypes();
     CStringArray* omStrGetPOCHandlerPrototypes();*/
    CStringArray* omStrGetTimerHandlerPrototypes();
    CStringArray* omStrGetMessageHandlerPrototypes();
    // CStringArray* omStrGetPduHandlerPrototypes();
    int ReadString(const LPSTR lpBuffer, UINT nSize, LPSTR lpsz, UINT& nIndex);
    long m_lCurrentWarningLineNum;
    POSITION SetPosToFirstLine();
    int nGetDocumentSize();
    const char* pcGetLine(POSITION& rPosition);
    DWORD dwGetLineCount();
    void vInsertErrorMessageStruct();
    BOOL bInitBusSpecificInfo(SBUS_SPECIFIC_INFO& sBusSpecInfo);
    BOOL bGetBusSpecificInfo(SBUS_SPECIFIC_INFO& sBusSpecInfo);
    BOOL bAddFunctionPrototype(CString omStrFunton, BOOL bGCCExport);
    BOOL bAddFunctionWrappers(CString m_omStrSelectedItemText, CString omStrFunctionName);
    virtual void GetWrapperDeclDef(ETYPE_BUS, CString strFunName, CString& strDec, CString& strDef);
    BOOL bAddGCCExportPrototype(CString& omStrFunction);
    virtual ~CFunctionEditorDoc();
    BOOL bCreateNewDocument(CString& omPath, CGlobalObj*);
    void vInsertErrorMessageStruct(CString& omTextLine);

    //Notify File is Modified.
    void ReloadModifiedFile();
    //It is set if file is modified externally.
    void SetNotifyReload(bool bNotifyReload);

    void ParseCppFile(std::string);
    void vReplaceStructVars(std::string& readLine);
    void vReplaceDataBytes(std::regex, std::string, std::string omReplaceStr, std::string& readLine);
    CString GetApplicationVersion();
    virtual BOOL bUpdateProtocolDetails(CString& omstrProtocol);
    virtual void vAddBusSpecificHandlers(CString omTextLine);
    virtual void vSaveProtocolSpecificInfo(CString&);
    void vUpdateDEFFile(CString omStrFileName);
    void vAddHandlersToDEFFile();
    virtual void vAddBusSpecificHandlersToDEFFile();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
private:
    CStringArray m_omErrorHandlerArray;
    CStringArray m_omDLLHandlerArray;
    CStringArray m_omBusEventsHandlerArray;
    CStringArray m_omEventIndArray;
    int m_nMaxLineLength;

    DWORD m_dwSourceCodeLineNo;

    CStringArray m_omTimerHandlerArray;
    CStringArray m_omKeyHandlerArray ;

    CStringArray m_omUtilityFuncArray ;

    CStringArray m_omIncludeFileArray ;


    // To get the application version

    void vInitialiseBusSpecStructure(CString& omStrTemp, int unChannel,
                                     SMSG_NAME_CODE& sMsgNameCode, BOOL bInitData);
    bool bCheckIfOldCppFile(CString& );
protected:

    // Generated message map functions
protected:

    CStringArray m_omMessageHandlerArray;
    //CStringArray m_omPDUHandlerArray;
    //{{AFX_MSG(CFunctionEditorDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
