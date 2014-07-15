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
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

enum eSECTION_ID // These IDs should be maintained with load sequence
{
    DATABASE_SECTION_ID = 0x0,
    BUS_STATISTICS_SECTION_ID,
    DATABASE_SECTION_J1939_ID,
    DATABASE_SECTION_FLEXRAY,
    DATABASE_SECTION_LIN_ID,
    DIL_SECTION_ID,
    DIL_SECTION_LIN_ID,
    FILTER_SECTION_ID,
    FILTER_SECTION_LIN_ID,
    MAINFRAME_SECTION_ID,
    SIGWATCH_SECTION_ID,
    SIGWATCH_SECTION_J1939_ID,
    SIGWATCH_SECTION_LIN_ID,
    GRAPH_SECTION_ID,
    LOG_SECTION_ID,
    LOG_SECTION_J1939_ID,
    LOG_SECTION_LIN_ID,
    SIMSYS_SECTION_ID,
    SIMSYS_SECTION_J1939_ID,
    REPLAY_SECTION_ID,
    MSGWND_SECTION_ID,
    MSGWND_SECTION_J1939_ID,
    MSGWND_SECTION_LIN_ID,
    TXWND_SECTION_ID,
    WAVEFORMDATA_SECTION_ID,
    TEST_SETUP_EDITOR_SECTION_ID,
    TEST_SUITE_EXECUTOR_SECTION_ID,
    FLEXRAY_TXWND,
    MSGWND_SECTION_FLEXRAY_ID,
    FLEXRAY_CLUSTER_CONFIG,
    SIMSYS_SECTION_LIN_ID,
    LIN_CLUSTER_CONFIG,
    LIN_TXWND,
    LIN_SCHEDULE_CONFIG,
    SECTION_TOTAL
};

//This Names shold be maintained with eSECTION_ID sequence
static char SectionName[SECTION_TOTAL][MAX_PATH] =
{
    "DatabaseConfigSectionName",
    "DatabaseConfigSectionNameJ1939",
    "DilConfigSectionName",
    "DilConfigSectionNameLIN",
    "FilterConfigSectionName",
    "MainFrameConfigSectionName",
    "SigWatchConfigSectionName",
    "SigWatchConfigSectionNameJ1939",
    "GraphConfigSectionName",
    "LogConfigSectionName",
    "LogConfigSectionNameJ1939",
    "SimSysConfigSectionName",
    "SimSysConfigSectionNameJ1939",
    "ReplayConfigSectionName",
    "MsgWndConfigSectionName",
    "MsgWndConfigSectionNameJ1939",
    "MsgWndConfigSectionNameLIN",
    "TxWndConfigSectionName",
    "TxWndConfigSectionNameLIN",
    "WaveformConfigSectionName",
    "BusStatsConfigSectionName",
    "TestSetupEditorSectionName",
    "TestSuiteExecutorSectionName"
};

// XML GENERATION DEFINES
#define DEF_BUSMASTER_INIT_CONFIG       "Busmaster_Init_Config"
#define DEF_TOOLBAR_POSITION_DEFAULT    "Toolbar_Position_Default"
#define DEF_TOOLBAR_POS_USER_DEFINED    "Toolbar_Position_User_Defined"
#define DEF_TOOLBAR_MSG_WND             "Toolbar_Msg_Wnd"
#define DEF_TOOLBAR_J1939               "Toolbar_J1939"
#define DEF_TOOLBAR_CONFIG              "Toolbar_Config"
#define DEF_TOOLBAR_CAN_DB              "Toolbar_CAN_DB"
#define DEF_TOOLBAR_NODE_SIM            "Toolbar_Node_Sim"
#define DEF_TOOLBAR_MAIN                "Toolbar_Main"
#define DEF_TOOLBAR_DOCKING             "Docked"
#define DEF_TOOLBAR_ALIGNMENT           "Alignment"
#define DEF_BUSMASTER_CONFIGURATION     "BUSMASTER_CONFIGURATION"
#define DEF_GLOBAL_CONFIGURATION        "Global_Configuration"
#define DEF_MODULE_CONFIGURATION        "Module_Configuration"
#define DEF_CAN_DATABASE_FILES          "CAN_Database_Files"
#define DEF_FLEXRAY_CLUSTER_CONFIG      "FLEX_Cluster_Config"
#define DEF_J1939_DATABASE_FILES        "J1939_Database_Files"
#define DEF_FILE_PATH                   "FilePath"
#define DEF_CHANNEL_NUMBER              "channelNumber"
#define DEF_CAN_DIL_SECTION             "CAN_DIL_Section"
#define DEF_DRIVER_NAME                 "DriverName"
#define DEF_CONTROLLER_MODE             "ControllerMode"
#define DEF_CONTROLLER_SETTINGS         "ControllerSettings"
#define DEF_CHANNEL                     "Channel"
#define DEF_LIN_CLUSTER_CONFIG      "LIN_Cluster_Config"
