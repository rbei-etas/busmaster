#pragma once
#include "TxWindowEthernet_stdafx.h"
#include "EditExtended.h"


CEditExtended::CEditExtended(void)
{
}


CEditExtended::~CEditExtended(void)
{
}

BEGIN_MESSAGE_MAP(CEditExtended, CEdit)
    //{{AFX_MSG_MAP(CRadixEdit)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CEditExtended::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			int nStart, nEnd;
			GetSel(nStart, nEnd);
			UINT nChar = (UINT)pMsg -> wParam;
			if ( ((nChar >= '0') && (nChar <= '9'))
				|| ((nChar >= 'A') && (nChar <= 'F'))
				|| ((nChar >= 'a') && (nChar <= 'f'))
				)
			{
				CString str;
				GetWindowText(str);
				//Check the boundary, if out of bounds then return
				if((str.GetLength() == 0)||(nStart >= (str.GetLength() -1)))
				{
					return S_FALSE;
				}
				char c = str.GetAt(nStart);
				//if ' ' then skip
				if(c == ' ')
				{
					c = str.GetAt(++nStart);
				}

				//Increase the cursor position so that it points to the next position after setting the character
				str.SetAt(nStart++, nChar);
				SetWindowText(str);
				SetSel(nStart, nStart);

				if(nEnd >nStart)
				{
					for(int i = nStart; i < nEnd; i++)
					{
						c = str.GetAt(i);
						if(c == ' ')
						{
							continue;
						}
						str.SetAt(i, nChar);
					}
					SetWindowText(str);
					SetSel(nEnd, nEnd);
				}
				
			}
			else if(nChar == VK_DELETE)
			{
				CString str;
				GetWindowText(str);
				if(nStart >= (str.GetLength() -1))
				{
					return S_FALSE;
				}
				char c = str.GetAt(nStart);

				if(c == ' ')
				{
					c = str.GetAt(++nStart);
				}
				str.SetAt(nStart, '0');

				SetWindowText(str);
				nStart++;
				SetSel(nStart, nStart);
			}
			else if (nChar == VK_BACK)
			{
				if(nStart == 0)
					return S_FALSE;
				CString str;
				GetWindowText(str);
				char c = str.GetAt(--nStart);
				if(c == ' ')
				{
					c = str.GetAt(--nStart);
				}
				str.SetAt(nStart, '0');
				SetWindowText(str);
				SetSel(nStart, nStart);
			}
			else if((nChar == VK_LEFT)||(nChar == VK_UP)
				||(nChar == VK_RIGHT)||(nChar == VK_DOWN))
			{
				break;
			}

			return S_FALSE;
		}
		break;
	}
	return CEdit::PreTranslateMessage(pMsg);
}

