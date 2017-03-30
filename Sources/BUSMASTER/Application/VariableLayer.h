#pragma once
#include "IVariableLayer.h"
#include "IBusmasterPlugin.h"
#include "IBusmasterPluginInterface.h"

#include "Utility\Utility_Thread.h"
#include "MsgBufFSE.h"
#include "BaseDIL_CAN.h"
#include "BaseDIL_LIN.h"
#include "BaseDIL_FLEXRAY.h"

#include "CANSignalExtractor.h"

#include <string>
#include <vector>
namespace VariableManager
{
	class VariableLayer : public IVariableLayer, public IBusmasterPlugin, public IEvent, public IDbChangeListner
	{
		CANSignalExtractor mCanSignalExtractor;
		IBusmasterPluginInterface* mBmPluginInterface = nullptr;

		CBaseDIL_CAN* mCanService = nullptr;
		CMsgBufFSE<STCANDATA> m_ouCanBufFSE;

		CBaseDIL_FLEXRAY* mFrService = nullptr;
		CBaseDIL_LIN* mLinService = nullptr;
		CPARAM_THREADPROC m_ouReadThread;
	public:
		VariableLayer();
		~VariableLayer();

		bool isVariableExists(std::string& variablePath);
		int registerVariable(std::string& variablePath, IVariableChangeListner*, bool forTx, bool onUpdateOnly);
		int UnRegisterVariable(std::string& variablePath, IVariableChangeListner*);
		int setVariableValue(VariableData* value, bool isPhysicalValue);

		int onBusmasterEvent(eBusmaster_Event, void*);

        void OnDataBaseChange(bool mIsAdded, const DBChangeInfo&);

		void setBusmasterInterface(IBusmasterPluginInterface*);
		void getMenuInterface(IMenuInterface**);
		void getNotifySink(IEvent**);
		void getConfiguration(xmlNodePtr& pNodePtr);       
		void setConfiguration(const xmlDocPtr);            
		void getConnectPoint(IBusmasterPluginConnection**);
		ILicenseProvider *getLicenseProvider();
		
	private:
		int StartReadThread();
		int StopReadThread();
		void handleBusEvent(Event_Bus_Staus busEvent);
		static DWORD WINAPI ReadThreadProc(LPVOID pVoid);
		int ReadCANDataBuffer();
	};
};

