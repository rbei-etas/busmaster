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
 * \file      Comment.h
 * \brief     CComment class.
 * \author    RBIN/EBS1 - Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CComment class.
 */
/**
* \file       Comment.h
* \brief      CComment class.
* \authors    Mahesh B S
* \date       17.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#if !defined(AFX_COMMENT_H__F394E0E9_F6B7_43F5_AB3F_428BBF638B46__INCLUDED_)
#define AFX_COMMENT_H__F394E0E9_F6B7_43F5_AB3F_428BBF638B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

class CComment
{

public:
    //data members
    unsigned int m_msgID;
    char m_msgType;
    CString m_comment;
    CString m_elementName;

    //member functions
    CComment();
    CComment& operator=(CComment& tCmt);
    virtual ~CComment();
    void Format_netComment(CStdioFile &fileInput,CList<CComment,CComment&>& m_listComment);
    void Format_nodeComment(CStdioFile &fileInput,CList<CComment,CComment&>& m_listComment);
    void Format_msgComment(CStdioFile &fileInput,CList<CComment,CComment&>& m_listComment);
    void Format_sigComment(CStdioFile &fileInput,CList<CComment,CComment&>& m_listComment);
};

#endif // !defined(AFX_COMMENT_H__F394E0E9_F6B7_43F5_AB3F_428BBF638B46__INCLUDED_)
