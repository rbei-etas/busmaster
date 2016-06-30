#pragma once
#include "..\Kernel\ProtocolDefinitions\ProtocolsDefinitions.h"
enum eBusmaster_Event
{
    Bus_Status,                         // struct Event_Bus_Staus{}
    network_config_changed,             // ETYPE_BUS bus;
    driver_selection_changed,           // ETYPE_BUS bus;

    Channel_Added,                      // struct Event_Channel_Added{}
    Channel_Updated,                    // struct Event_Channel_Updated{}

    plugin_load_completed,              // nothing
    plugin_loaded,                      // Feature use
    plugin_unloaded,                    // Feature use
    new_configuration_loading,          // null
    new_configuration_loaded,           // null

    display_numeric_mode_changed,       // enum Numeric_mode

    key_pressed,

    busmaster_exit,
};


struct Event_Bus_Staus
{
    ETYPE_BUS mBus;
    eBUSEVENT mEventType;
};


struct Event_Channel_Added
{
    ETYPE_BUS bus;
    int mChannel;
    int mTotalChannels;
};

struct Event_Channel_Updated
{
    ETYPE_BUS mBus;
    int mChannel;
};

enum eNumeric_mode
{
    DECICAL_MODE,
    HEXADECIMAL_MODE
};