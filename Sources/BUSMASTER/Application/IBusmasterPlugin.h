#pragma once
#include "BusmasterEvents.h"
#include "IBusmasterPluginInterface.h"
#include "IBusmasterPluginConnection.h"
#include <libxml\parser.h>
#include "UI\Menus.h"


class IEvent
{
public:
    virtual int onBusmasterEvent( eBusmaster_Event, void* ) =0 ;
};

class IMenuItem
{
public:
    virtual ~IMenuItem() {}
    virtual void enableItem( bool ) = 0;
    virtual void checkItem( bool ) = 0;
    virtual void setName( char* ) = 0;
    virtual void getName( char*, int maxlength ) = 0;
};
class IMenuInterface
{
public:
    virtual void OnMenuItemClicked( const char* menuId )=0;
    virtual void updateMenuItem(const char* menuId, IMenuItem*) = 0;
};



class PluginInfo
{
public:
    char mPluginId[256];            //Required and should bu unique like (recommendded busmaster.plugin.<bus>.<feature>.<etc>) busmaster.plugin.can.signalwatch
    char mVersion[15];                 //Version of plugin.
    char mDescription[512];         //description.
    std::list<PluginMenu> mPluginMenus;
};
class IBusmasterPlugin
{
public:
    virtual void setBusmasterInterface( IBusmasterPluginInterface* ) = 0;
    virtual void getMenuInterface( IMenuInterface** ) = 0;
    virtual void getNotifySink( IEvent** ) = 0;
    virtual void getConfiguration(xmlNodePtr& pNodePtr) = 0;            //TODO
    virtual void setConfiguration(const xmlDocPtr) = 0;            //TODO
    virtual void getConnectPoint(IBusmasterPluginConnection**) = 0;     //TODO :: For Inter plugin communication
};
