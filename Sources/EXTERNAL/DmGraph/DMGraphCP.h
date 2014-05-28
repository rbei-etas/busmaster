/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      DMGraphCP.h: Declaration of the CProxy_IDMGraphCtrlEvents class
//
// CLASS NAME
//      CProxy_IDMGraphCtrlEvents
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila created by the wizard
//

#pragma once

template <class T>
class CProxy_IDMGraphCtrlEvents : public IConnectionPointImpl<T, &DIID__IDMGraphCtrlEvents, CComDynamicUnkArray>
{
    //Warning this class may be recreated by the wizard.
public:
    VOID Fire_CursorPosition(DOUBLE x, DOUBLE y, short shCursorID)
    {
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[3];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != nullptr)
            {
                pvars[2] = x;
                pvars[1] = y;
                pvars[0] = shCursorID;
                DISPPARAMS disp = { pvars, nullptr, 3, 0 };
                pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, nullptr, nullptr, nullptr);
            }
        }
        delete[] pvars;

    }
    VOID Fire_TrackModeChanged(TrackModeState lNewState)
    {
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[1];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != nullptr)
            {
                pvars[0] = lNewState;
                DISPPARAMS disp = { pvars, nullptr, 1, 0 };
                pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, nullptr, nullptr, nullptr);
            }
        }
        delete[] pvars;

    }
    VOID Fire_Click()
    {
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != nullptr)
            {
                DISPPARAMS disp = { nullptr, nullptr, 0, 0 };
                pDispatch->Invoke(DISPID_CLICK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, nullptr, nullptr, nullptr);
            }
        }

    }
};
