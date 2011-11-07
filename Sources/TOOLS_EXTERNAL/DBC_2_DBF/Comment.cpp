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
 * \file      Comment.cpp
 * \brief     Implementation file for the Comment class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the Comment class.
 */

#include "StdAfx.h"
#include "Comment.h"

/**
 * \brief Constructor
 */
CComment::CComment()
{
    m_msgType = '\0';
    m_elementName = "\0";
    m_comment = "\0";
}

/**
 * \brief Destructor
 */
CComment::~CComment()
{

}
