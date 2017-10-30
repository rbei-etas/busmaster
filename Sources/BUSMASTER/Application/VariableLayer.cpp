#include "stdafx.h"
#include "VariableLayer.h"

#include <sstream>

namespace VariableManager
{
	VariableLayer::VariableLayer()
	{
	}


	VariableLayer::~VariableLayer()
	{
	}

	bool VariableLayer::isVariableExists(std::string& variablePath)
	{
		std::vector<std::string> tokens;

		if (variablePath.find(":CAN:") != -1)
		{
			return mCanSignalExtractor.bIsVariableExists(variablePath);
		}
		else if (variablePath.find(":LIN:") != -1)
		{
			return mLINSignalExtractor.bIsVariableExists(variablePath);
		}
	}

	int VariableLayer::getVariableType(std::string variablePath, unsigned int& unType)
	{
		std::vector<std::string> tokens;

		if (variablePath.find(":CAN:") != -1)
		{
			return mCanSignalExtractor.getVariableType(variablePath, unType);
		}
		else if (variablePath.find(":LIN:") != -1)
		{
			return mLINSignalExtractor.getVariableType(variablePath, unType);
		}
	}
	int VariableLayer::isSignalValueValid(std::string variablePath, double& physical)
	{
		std::vector<std::string> tokens;

		if (variablePath.find(":CAN:") != -1)
		{
			return mCanSignalExtractor.isSignalValueValid(variablePath, physical);
		}
		else if (variablePath.find(":LIN:") != -1)
		{
			return mLINSignalExtractor.isSignalValueValid(variablePath, physical);
		}
	}
	int VariableLayer::registerVariable(std::string& variablePath, IVariableChangeListner* listner, bool forTx, bool onUpdateOnly)
	{
		std::vector<std::string> tokens;

		if (variablePath.find(":CAN:") != -1)
		{
			return mCanSignalExtractor.registerVariable(variablePath, listner);
		}
		else if (variablePath.find(":LIN:") != -1)
		{
			return mLINSignalExtractor.registerVariable(variablePath, listner);
		}		
	}
    int VariableLayer::UnRegisterVariable(std::string& variablePath, IVariableChangeListner* listner)
	{
		if (variablePath.find(":CAN:") != -1)
		{
			return mCanSignalExtractor.unRegisterVariable(variablePath, listner);
		}
		else if (variablePath.find(":LIN:") != -1)
		{
			return mLINSignalExtractor.unRegisterVariable(variablePath, listner);
		}
	}
	int VariableLayer::setVariableValue(VariableData* value, bool isPhysicalValue)
	{
		if (nullptr != value)
		{
			if (value->mVariablePath.find(":CAN:") != -1)
			{
				return mCanSignalExtractor.setVariableValue(value, isPhysicalValue);
			}
			else if (value->mVariablePath.find(":LIN:") != -1)
			{
				return mLINSignalExtractor.setVariableValue(value, isPhysicalValue);
			}
		}		
	}

	void VariableLayer::setBusmasterInterface(IBusmasterPluginInterface* bmPluginInterface)
	{
		mBmPluginInterface = bmPluginInterface;

		IBMNetWorkGetService* dbService;
		mBmPluginInterface->getDbService(&dbService);
		mCanSignalExtractor.setBmDatabaseInterface(dbService);
		mLINSignalExtractor.setBmDatabaseInterface(dbService);

        dbService->ManageClientForDbChanges(this, true);

		IBusService* busService;
		//CAN
		mBmPluginInterface->getDilService(ETYPE_BUS::CAN, &busService);
		mCanService = (CBaseDIL_CAN*)busService;

		//LIN
		mBmPluginInterface->getDilService(ETYPE_BUS::LIN, &busService);
		mLinService = (CBaseDIL_LIN*)busService;

		//FlexRay
		mBmPluginInterface->getDilService(ETYPE_BUS::FLEXRAY, &busService);
		mFrService = (CBaseDIL_FLEXRAY*)busService;

	}
	void VariableLayer::getMenuInterface(IMenuInterface** menuInterface)
	{
		*menuInterface = nullptr;
	}
	void VariableLayer::getNotifySink(IEvent** pEventSink)
	{
		*pEventSink = this;
	}
	void VariableLayer::getConfiguration(xmlNodePtr& pNodePtr)
	{

	}
	void VariableLayer::setConfiguration(const xmlDocPtr)
	{

	}
	void VariableLayer::getConnectPoint(IBusmasterPluginConnection** connectionPoint)
	{
		*connectionPoint = nullptr;
	}

	ILicenseProvider* VariableLayer::getLicenseProvider()
	{
		return nullptr;
	}
	int VariableLayer::onBusmasterEvent(eBusmaster_Event busEvent, void* eventData)
	{
		switch (busEvent)
		{
		case Bus_Status:
		{
			Event_Bus_Staus busEvent = *((Event_Bus_Staus*)eventData);
			handleBusEvent(busEvent);
		}
		break;
		case driver_selection_changed:
		{
			ETYPE_BUS busEvent = *((ETYPE_BUS*)eventData);

			if (busEvent == ETYPE_BUS::CAN)
			{
				StartReadThread();
			}
			else if (busEvent == ETYPE_BUS::LIN)
			{
				StartLINReadThread();
			}
		}
		case new_configuration_loaded:
		{

		}
		break;
		case display_numeric_mode_changed:
		{

		}
		break;
		case busmaster_exit:
		{
			StopReadThread();
			StopLINReadThread();
		}

		default:
			break;
		}
		return 0;
	}

	void VariableLayer::handleBusEvent(Event_Bus_Staus busEvent)
	{
		/*if (busEvent.mBus != ETYPE_BUS::CAN)
		{
			return;
		}*/
		switch (busEvent.mEventType)
		{
		case ON_CONNECT:
			
			break;
		case ON_PRE_CONNECT:
			//StartReadThread();
			break;
		case ON_DISCONNECT:
			//StopReadThread();
			if (busEvent.mBus == ETYPE_BUS::CAN)
			{
				mCanSignalExtractor.handleBusEvent(busEvent);
			}
			else if (busEvent.mBus == ETYPE_BUS::LIN)
			{
				mLINSignalExtractor.handleBusEvent(busEvent);
			}
			break;
		default:
			break;
		}
	}

    void VariableLayer::OnDataBaseChange(bool mIsAdded, const DBChangeInfo& info)
    {
		if (info.mBusType == CAN)
		{
			mCanSignalExtractor.onDatabaseChanged(info.mBusType, info.mDbPath, info.mChannel, mIsAdded);
		}      
		if (info.mBusType == LIN)
		{
			mLINSignalExtractor.onDatabaseChanged(info.mBusType, info.mDbPath, info.mChannel, mIsAdded);
		}
    }

	int VariableLayer::StartReadThread()
	{
		unsigned long clientId = 0;
		IBusService* busService;

		mBmPluginInterface->getDilService(CAN, (IBusService**)&busService);
		CBaseDIL_CAN* canDil = (CBaseDIL_CAN*)busService;

		//mBusMasterInterface->registerOnBus( CAN, &m_ouReadThread, true, clientId );
		canDil->DILC_RegisterClient(TRUE, clientId, CAN_MONITOR_NODE);
		canDil->DILC_ManageMsgBuf(MSGBUF_ADD, clientId, &m_ouCanBufFSE);
		m_ouReadThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();
		m_ouReadThread.m_pBuffer = this;
		mCanSignalExtractor.setCANService(canDil, clientId);
		m_ouReadThread.bStartThread(ReadThreadProc);
		return S_OK;
	}
	int VariableLayer::StopReadThread()
	{
		mCanSignalExtractor.setCANService(nullptr, 0);
		m_ouReadThread.bTerminateThread();
		return 0;
	}
	int VariableLayer::ReadCANDataBuffer()
	{
		while (m_ouCanBufFSE.GetMsgCount() > 0)
		{
			static STCANDATA sCanData;
			INT Result = m_ouCanBufFSE.ReadFromBuffer(&sCanData);
			mCanSignalExtractor.onMessageRecieved(sCanData);
		}
		return 0;
	}

	DWORD WINAPI VariableLayer::ReadThreadProc(LPVOID pVoid)
	{
		CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*)pVoid;
		if (pThreadParam == nullptr)
		{
			return (DWORD)-1;
		}
		VariableLayer* pSWCan = (VariableLayer*)pThreadParam->m_pBuffer;
		if (pSWCan == nullptr)
		{
			return (DWORD)-1;
		}
		bool bLoopON = true;
		pThreadParam->m_unActionCode = INVOKE_FUNCTION;
		while (bLoopON)
		{
			WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
			switch (pThreadParam->m_unActionCode)
			{
			case INVOKE_FUNCTION:
			{
				pSWCan->ReadCANDataBuffer(); // Retrieve message from the driver
			}
			break;
			case EXIT_THREAD:
			{
				bLoopON = false;
			}
			break;
			case CREATE_TIME_MAP:
			{
				//Nothing at this moment
			}
			break;
			default:
			case INACTION:
			{
				// nothing right at this moment
			}
			break;
			}
		}
		SetEvent(pThreadParam->hGetExitNotifyEvent());
		return 0;
	}

	// LIN BUS
	int VariableLayer::StartLINReadThread()
	{
		unsigned long clientId = 0;
		IBusService* busService;

		mBmPluginInterface->getDilService(LIN, (IBusService**)&busService);
		CBaseDIL_LIN* canDil = (CBaseDIL_LIN*)busService;

		//mBusMasterInterface->registerOnBus( CAN, &m_ouReadThread, true, clientId );
		canDil->DILL_RegisterClient(TRUE, clientId, LIN_MONITOR_NODE);
		canDil->DILL_ManageMsgBuf(MSGBUF_ADD, clientId, &m_ouLINBufFSE);
		m_ouLINReadThread.m_hActionEvent = m_ouLINBufFSE.hGetNotifyingEvent();
		m_ouLINReadThread.m_pBuffer = this;
		mLINSignalExtractor.setLINService(canDil, clientId);
		m_ouLINReadThread.bStartThread(ReadLINThreadProc);
		return S_OK;
	}
	int VariableLayer::StopLINReadThread()
	{
		mLINSignalExtractor.setLINService(nullptr, 0);
		m_ouLINReadThread.bTerminateThread();
		return 0;
	}
	int VariableLayer::ReadLINDataBuffer()
	{
		while (m_ouLINBufFSE.GetMsgCount() > 0)
		{
			static STLINDATA sLINData;
			INT Result = m_ouLINBufFSE.ReadFromBuffer(&sLINData);
			mLINSignalExtractor.onMessageRecieved(sLINData);
		}
		return 0;
	}

	DWORD WINAPI VariableLayer::ReadLINThreadProc(LPVOID pVoid)
	{
		CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*)pVoid;
		if (pThreadParam == nullptr)
		{
			return (DWORD)-1;
		}
		VariableLayer* pSWCan = (VariableLayer*)pThreadParam->m_pBuffer;
		if (pSWCan == nullptr)
		{
			return (DWORD)-1;
		}
		bool bLoopON = true;
		pThreadParam->m_unActionCode = INVOKE_FUNCTION;
		while (bLoopON)
		{
			WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
			switch (pThreadParam->m_unActionCode)
			{
			case INVOKE_FUNCTION:
			{
				pSWCan->ReadLINDataBuffer(); // Retrieve message from the driver
			}
			break;
			case EXIT_THREAD:
			{
				bLoopON = false;
			}
			break;
			case CREATE_TIME_MAP:
			{
				//Nothing at this moment
			}
			break;
			default:
			case INACTION:
			{
				// nothing right at this moment
			}
			break;
			}
		}
		SetEvent(pThreadParam->hGetExitNotifyEvent());
		return 0;
	}

	void VariableLayer::HandleImportInstruments(std::string& variablePath)
	{
		std::vector<std::string> tokens;

		if (variablePath.find(":CAN:") != -1)
		{
			mCanSignalExtractor.HandleImportInstruments();
		}
		else if (variablePath.find(":LIN:") != -1)
		{
			mLINSignalExtractor.HandleImportInstruments();
		}
	}

};

