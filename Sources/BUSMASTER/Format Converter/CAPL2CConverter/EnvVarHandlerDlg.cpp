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
 * \file      CEnvVarHandlerDlg.cpp
 * \brief     Implementation file for CList class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CList class
 */


#include "CAPL2CConverter_stdafx.h"

#include "SymbolTable.h"
#include "ConstString.h"
#include "EnvVarHandlerDlg.h"


extern char cIndex2;       //to store the acSptdKey (needed)
extern char acSptdKey[defINT_SizeofacAltKeyArray];//to store supported keys
extern CSymbolTable ouUnSptdKey;
extern CString ExceptionFormat;
extern char acAltKey[254];  //to store the alternate keys
extern int m; //neccessary
extern FILE* pFileLog;                    //for log file

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvVarHandlerDlg dialog


CEnvVarHandlerDlg::CEnvVarHandlerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CEnvVarHandlerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEnvVarHandlerDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    //  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CEnvVarHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEnvVarHandlerDlg)
    DDX_Control(pDX, IDC_LSTB_UNSK, m_UnSupKeys);
    DDX_Control(pDX, IDC_LSTB_AVLK, m_AltKeys);
    DDX_Control(pDX, IDC_LSTB_ALTK, m_EquiKeys);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvVarHandlerDlg, CDialog)
    //{{AFX_MSG_MAP(CEnvVarHandlerDlg)
    ON_BN_CLICKED(IDC_CBTN_ADD, OnAdd)
    ON_BN_CLICKED(IDC_CBTN_OK, OnTerminate)
    ON_BN_CLICKED(IDC_CBTN_RMV, OnRemove)
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvVarHandlerDlg message handlers

/*******************************************************************************
 Function Name    : OnInitDialog
 Input(s)         :     -
 Output           :     -
 Functionality    : This will populate the first list box with unsupported
                    keys and environment handlers and the second list box
                    with available keys.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
BOOL CEnvVarHandlerDlg::OnInitDialog()
{
    try
    {
        int Flag = 0;
        int m = 0;

        char allkey[]= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
                        'o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E',
                        'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V',
                        'W','X','Y','Z','0','1','2','3','4','5','6','7','8','9',0
                       };
        CDialog::OnInitDialog();

        for(m = 0; m < ouUnSptdKey.nGetSize(); m++)
        {
            m_UnSupKeys.AddString( ouUnSptdKey.omGetAt(m) );
        }

        for( m = 0; m < defINT_SizeofacAltKeyArray ; m++ )
        {
            Flag = 0;
            for( int p = 0; p <= cIndex2-1; p++)
            {
                if(allkey[m] == acSptdKey[p])
                {
                    Flag = 1;
                    break;
                }
            }
            if( Flag == 0)
            {
                m_AltKeys.AddString( CString( allkey[m] ) );
            }
        }

        m_UnSupKeys.SetCurSel(0);
        m_AltKeys.SetCurSel(0);

        if( m_AltKeys.GetCount() == 0)
        {
            //if no key is available
            GetDlgItem( IDC_CBTN_ADD )->EnableWindow(FALSE);
            ShowWindow(SW_SHOW);
            MessageBox(_("No Key is avilable"),NULL,MB_OK);

        }
    }

    catch(...)
    {
        CString cs;

        cs.Format(ExceptionFormat,"\"OnInitDialog\"",__FILE__,__LINE__);
        MessageBox(cs);
        exit(0);
    }

    return TRUE;

}
/*******************************************************************************
 Function Name    : OnAdd
 Input(s)         :     -
 Output           :     -
 Functionality    : This will take selected element from first two list and add
                    it to the third one.The element selected from the second
                    list will be added to acAltKey array at the place where the
                    first list element is stored in acUnSptdKey array.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 23.04.2004
 Modifications    :
*******************************************************************************/
void CEnvVarHandlerDlg::OnAdd()
{
    int m = 0;

    CString uns,alt,uns1;

    m = m_UnSupKeys.GetCurSel();

    m_UnSupKeys.GetText( m,uns );//store selected element from first list
    m_UnSupKeys.GetText( m,uns1 );
    m_UnSupKeys.DeleteString( m );//delete it
    m = m_AltKeys.GetCurSel();

    m_AltKeys.GetText( m,alt );//store selected element from second list
    m_AltKeys.DeleteString( m );

    uns = uns+" --->  ";
    uns = uns + alt;
    m_EquiKeys.AddString( uns );//add both to third list

    acAltKey[ouUnSptdKey.nFind(uns1)] = alt[0];//add the key to acAltKeylist

    m_UnSupKeys.SetCurSel(0);//set first item selected
    m_AltKeys.SetCurSel(0);
    m_EquiKeys.SetCurSel(0);
    if(m_UnSupKeys.GetCount() == 0 || m_AltKeys.GetCount() == 0 )
    {
        GetDlgItem( IDC_CBTN_ADD )->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem( IDC_CBTN_ADD )->EnableWindow(TRUE);
    }
    GetDlgItem( IDC_CBTN_RMV )->EnableWindow(TRUE);
}
/*******************************************************************************
 Function Name    : OnRemove
 Input(s)         :     -
 Output           :     -
 Functionality    : This will will remove the selected item from the third list
                    and will put the first element in the first list and second
                    element in the second list.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 23.04.2004
 Modifications    :
*******************************************************************************/
void CEnvVarHandlerDlg::OnRemove()
{
    CString ekey;
    if((m = m_EquiKeys.GetCurSel()) != LB_ERR)
    {
        GetDlgItem( IDC_CBTN_ADD )->EnableWindow(TRUE);
        m_EquiKeys.GetText( m,ekey );
        int k = ekey.Find(' ',0);
        if( k != -1)
        {
            m_UnSupKeys.AddString(ekey.Left(k));
            m_AltKeys.AddString(ekey.Right(1));
        }

        acAltKey [ ouUnSptdKey.nFind( ekey.Left( k)) ] = -1;

        m_EquiKeys.DeleteString(m);

        m_UnSupKeys.SetCurSel(0);
        m_AltKeys.SetCurSel(0);
        m_EquiKeys.SetCurSel(0);
    }

    if(m_EquiKeys.GetCount()==0)
    {
        GetDlgItem(IDC_CBTN_RMV)->EnableWindow(FALSE);
    }
}
/*******************************************************************************
 Function Name    : OnTerminate
 Input(s)         :     -
 Output           :     -
 Functionality    : This will take first item from list one and first item from
                    list two to make the default selection of available keys for
                    the unsupported keys.This will write the user intervention
                    for the alternate keys in the log file also.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 27.04.2004
 Modifications    :
*******************************************************************************/
void CEnvVarHandlerDlg::OnTerminate()
{
    CString buffer;
    CString uns,alt,uns1;

    if(m_UnSupKeys.GetCount()>0)
    {
        if ( m_AltKeys.GetCount() > 0 )
        {
            MessageBox(defSTR_Warning6,MB_OK);
        }
        while(m_UnSupKeys.GetCount()!=0)
        {
            if(m_AltKeys.GetCount()>0)
            {
                m_UnSupKeys.GetText(0,uns);
                m_UnSupKeys.GetText(0,uns1);
                m_UnSupKeys.DeleteString(0);

                m_AltKeys.GetText(0,alt);
                m_AltKeys.DeleteString(0);

                uns = uns+" --->  ";
                uns = uns + alt;
                m_EquiKeys.AddString(uns);

                acAltKey[ ouUnSptdKey.nFind(uns1) ] = alt[0];
            }
            else
            {
                m_UnSupKeys.DeleteString(0);
            }
        }
    }

    /*if( m_EquiKeys.GetCount() > 0 )
    {
        fprintf(pFileLog,"---***********************************---");
        fprintf(pFileLog,"\n---*****CEnvVarHandlerDlg Of User Intervention*****---");
        fprintf(pFileLog,"\n---***********************************---");
        fprintf(pFileLog,"\n Unsupported Key -->Equivalent Key");
        fprintf(pFileLog,"\n ---------------    ---------------\n");
    }

    for(int m = 0; m <= m_EquiKeys.GetCount()-1; m++ )
    {
        m_EquiKeys.GetText( m,buffer );
        fprintf(pFileLog,"\t%s\n",buffer);
    }*/

    m_UnSupKeys.ResetContent();
    m_EquiKeys.ResetContent();
    m_AltKeys.ResetContent();
    CEnvVarHandlerDlg::OnOK();
}


void CEnvVarHandlerDlg::OnCancel()
{
    CEnvVarHandlerDlg::OnTerminate();
}

void CEnvVarHandlerDlg::OnClose()
{
    OnTerminate();
    CDialog::OnClose();
}



