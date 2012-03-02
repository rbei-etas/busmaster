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
 * \file      InterfaceGetter.h
 * \brief     Declaration file for getter functions for various interfaces.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for getter functions for various interfaces.
 */

#if !defined INTERFACEGETTER_H__INCLUDED_
#define INTERFACEGETTER_H__INCLUDED_

// All forward declarations
class CBaseFrameProcessor_CAN;
class CBaseFrameProcessor_J1939;
class CMainFrame;
class CFlags;
class CBaseDIL_CAN;
class CBaseDILI_J1939;
class CBaseBusStatisticCAN;
class CBaseNodeSim;
class CMsgSignal;

// External function prototypes
extern CBaseFrameProcessor_CAN* GetICANLogger(void);
extern CBaseFrameProcessor_J1939* GetIJ1939Logger(void);
extern CMainFrame* GetIMainFrame(void);
extern CFlags* GetIFlags(void);
extern CBaseDIL_CAN* GetICANDIL(void);
extern CBaseDILI_J1939* GetIJ1939DIL(void);
extern CBaseBusStatisticCAN* GetICANBusStat(void);
extern CBaseNodeSim* GetICANNodeSim(void);
extern CBaseNodeSim* GetIJ1939NodeSim(void);
extern CMsgSignal* GetIMsgDB(void);
extern DWORD dwGetMonitorClientID(void);


#endif // INTERFACEGETTER_H__INCLUDED_